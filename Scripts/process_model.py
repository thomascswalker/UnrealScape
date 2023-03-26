import logging
from typing import List
from pymxs import runtime as rt
from qtmax import GetQMaxMainWindow
import os
import sys

from PySide2.QtWidgets import (
    QMainWindow,
    QSizePolicy,
    QTableWidgetItem,
    QMessageBox,
)
from PySide2.QtCore import QFile
from PySide2.QtUiTools import QUiLoader

from glob import glob

try:
    import const  # noqa
except ImportError:
    sys.path.append(os.path.dirname(__file__))

from logger import get_logger
from const import (
    ProjectPaths,
    get_item_list,
    get_source_model_data,
    format_name,
    ModelData,
)

SOURCE_MODEL_DATA: ModelData = get_source_model_data()


def gamma_correct(color: rt.Color, gamma: float = 1.0) -> rt.Color:
    r = int(pow((color.r / 255.0), (1.0 / gamma)) * 255.0)
    g = int(pow((color.g / 255.0), (1.0 / gamma)) * 255.0)
    b = int(pow((color.b / 255.0), (1.0 / gamma)) * 255.0)
    return rt.Color(r, g, b)


def attach(nodes: List[rt.Node]) -> rt.Node:
    while len(nodes) > 1:
        rt.ConvertToPoly(nodes[0])
        rt.PolyOp.Attach(nodes[0], nodes[1])
    return nodes[0]


def create_multi_mtl() -> rt.MultiMaterial:
    mtl = rt.MultiMaterial()
    mtl.MaterialIdList = rt.Array(*[1, 2, 3, 4])
    mtl.Names = rt.Array(*["Flat", "Metal", "SSS", "Translucent"])

    mtl.MaterialList[0].base_color = rt.Red
    mtl.MaterialList[1].base_color = rt.Green
    mtl.MaterialList[2].base_color = rt.Blue
    mtl.MaterialList[3].base_color = rt.White

    return mtl


class ModelProcessor:
    logger = get_logger(__name__, logging.DEBUG)
    current_name: str = ""
    current_node: rt.Node = None

    class ModelProcessorGui(QMainWindow):
        def __init__(self) -> None:
            super().__init__(parent=GetQMaxMainWindow())
            self.setWindowTitle("UnrealScape Model Processor")
            self.set_ui()
            self.set_connections()
            self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

        def set_ui(self):
            ui_file = QFile(f"{ProjectPaths.ScriptsDir}\\process_model.ui")
            ui_file.open(QFile.ReadOnly)

            loader = QUiLoader()
            self.ui = loader.load(ui_file)
            self.setCentralWidget(self.ui)

            # Populate source model list
            self.ui.et_model_source.setText(ProjectPaths.SourceModelDir)
            for item in SOURCE_MODEL_DATA:
                row = self.ui.tv_source_models.rowCount()
                item_id = QTableWidgetItem(item[0])  # id
                item_name = QTableWidgetItem(item[1])  # name
                item_type = QTableWidgetItem(item[2])  # type
                item_ids = QTableWidgetItem(item[3])  # model ids

                self.ui.tv_source_models.insertRow(row)
                self.ui.tv_source_models.setItem(row, ModelData.Id, item_id)
                self.ui.tv_source_models.setItem(row, ModelData.Name, item_name)
                self.ui.tv_source_models.setItem(row, ModelData.Type, item_type)
                self.ui.tv_source_models.setItem(row, ModelData.ModelIds, item_ids)

            # Populate game model list
            game_models: List[str] = glob(f"{ProjectPaths.StaticMeshesDir}\\**.fbx")
            for file in game_models:
                model_name = os.path.basename(file).split(".")[0]
                item = QTableWidgetItem(model_name)
                row = self.ui.tv_game_models.rowCount()
                self.ui.tv_game_models.insertRow(row)
                self.ui.tv_game_models.setItem(row, 0, item)

        def set_connections(self) -> None:
            self.ui.btn_import_source_model.clicked.connect(
                self.on_source_import_pressed
            )
            self.ui.btn_import_game_model.clicked.connect(self.on_game_import_pressed)
            self.ui.btn_bake_vertex_color.clicked.connect(self.on_bake_pressed)
            self.ui.btn_assign_multi_material.clicked.connect(self.on_assign_mtl_pressed)

            self.ui.btn_flat.clicked.connect(lambda: self.on_mtl_id_pressed(1))
            self.ui.btn_metal.clicked.connect(lambda: self.on_mtl_id_pressed(2))
            self.ui.btn_sss.clicked.connect(lambda: self.on_mtl_id_pressed(3))
            self.ui.btn_translucent.clicked.connect(lambda: self.on_mtl_id_pressed(4))

        def on_source_import_pressed(self) -> None:
            selected_rows = self.ui.tv_source_models.selectionModel().selectedRows()
            if len(selected_rows) == 0:
                QMessageBox.warning(self, "Selection error", "Please select a row.")
                return
            row = selected_rows[0].row()
            name_item = self.ui.tv_source_models.item(row, ModelData.Name)
            ModelProcessor.current_name = name_item.text()
            model_ids_item = self.ui.tv_source_models.item(row, ModelData.ModelIds)
            model_ids = model_ids_item.text().replace(" ", "").split(",")
            ModelProcessor.import_models(model_ids)

        def on_game_import_pressed(self) -> None:
            selected_rows = self.ui.tv_game_models.selectionModel().selectedRows()
            if len(selected_rows) == 0:
                QMessageBox.warning(self, "Selection error", "Please select a row.")
                return
            row = selected_rows[0].row()
            model_item = self.ui.tv_game_models.item(row, 0)
            model_name = model_item.text()
            filename = os.path.join(ProjectPaths.StaticMeshesDir, f"{model_name}.fbx")
            rt.ImportFile(filename, rt.Name("NoPrompt"), using=rt.FBXIMP)

        def on_export_pressed(self) -> None:
            selection = rt.GetCurrentSelection()
            if len(selection) != 1:
                QMessageBox.warning(
                    self, "Selection error", "Please select exactly one object."
                )
                return
            node = selection[0]
            ModelProcessor.export_model(node.name)

        def on_bake_pressed(self) -> None:
            selection = rt.GetCurrentSelection()
            if len(selection) == 0:
                QMessageBox.warning(
                    self, "Selection error", "Please select one or more objects."
                )
                return

            for obj in selection:
                ModelProcessor.bake_material_to_vtx_color(obj)

        def on_mtl_id_pressed(self, *args) -> None:
            id = int(args[0])

            selection = rt.GetCurrentSelection()
            if len(selection) != 1:
                QMessageBox.warning(
                    self, "Selection error", "Please select exactly one object."
                )
                return

            obj = selection[0]
            face_selection = rt.PolyOp.GetFaceSelection(obj)
            rt.PolyOp.SetFaceMatId(obj, face_selection, id)

        def on_assign_mtl_pressed(self) -> None:
            selection = rt.GetCurrentSelection()
            if len(selection) != 1:
                QMessageBox.warning(
                    self, "Selection error", "Please select exactly one object."
                )
                return

            obj = selection[0]
            obj.material = create_multi_mtl()

    @staticmethod
    def format_model_list(model_ids: str) -> List[int]:
        fmt_indexes = model_ids.replace(" ", "").split(",")
        return [int(i) for i in fmt_indexes]

    @staticmethod
    def import_model(index: int) -> None:
        filename = os.path.join(ProjectPaths.SourceModelDir, f"{index}.obj")
        if not os.path.exists(filename):
            ModelProcessor.logger.error(f"File {index} not found.")
            return
        rt.ImportFile(filename, rt.Name("NoPrompt"), using=rt.OBJIMP)

    @staticmethod
    def import_models(indexes: List[int]) -> rt.Node:
        for index in indexes:
            ModelProcessor.import_model(index)
        nodes = rt.Objects
        result = attach(nodes)
        if ModelProcessor.current_name != "":
            fmt_name = format_name(ModelProcessor.current_name)
            result.name = f"SM_{fmt_name}"
        ModelProcessor.current_node = result
        return result

    @staticmethod
    def export_model(name: str) -> None:
        new_name = format_name(name)
        filename = os.path.join(ProjectPaths.StaticMeshesDir, f"\\SM_{new_name}.fbx")
        ModelProcessor.logger.debug(f"Exporting model to {filename}")
        rt.ExportFile(filename, rt.Name("NoPrompt"), using=rt.FBXEXP)

    @staticmethod
    def get_indexes_from_name(name: str, filt: str) -> List[int]:
        for item in SOURCE_MODEL_DATA:
            if item[SOURCE_MODEL_DATA.Type] != filt:
                continue
            if name == item[SOURCE_MODEL_DATA.Name]:
                ids: str = item[SOURCE_MODEL_DATA.ModelIds]
                return ModelProcessor.format_model_list(ids)

        return []

    @staticmethod
    def get_mtl_color(mtl: rt.Material, face_id: int) -> rt.Color:
        if mtl[face_id]:
            face_mtl = mtl[face_id]
            if face_mtl:
                face_color = face_mtl.DiffuseColor
                return face_color
        return rt.Black

    @staticmethod
    def bake_material_to_vtx_color(node: rt.Node) -> None:
        # Convert to edit poly
        rt.ConvertToPoly(node)
        node.showVertexColors = True

        # Get the object's material
        multi_mtl = node.material
        for face_id in range(node.NumFaces):
            face_color = ModelProcessor.get_mtl_color(multi_mtl, face_id)
            ModelProcessor.logger.debug(f"Setting face {face_id} to {str(face_color)}")
            rt.PolyOp.SetFaceColor(node, 0, face_id + 1, face_color)
        node.material = None

    @staticmethod
    def mesh_exists(name: str) -> bool:
        name = format_name(name)
        mesh_file = os.path.join(ProjectPaths.AssetsDir, f"StaticMeshes\\SM_{name}.fbx")
        return os.path.exists(mesh_file)

    @staticmethod
    def process(name: str, overwrite: bool = False) -> None:
        if ModelProcessor.mesh_exists(name) and not overwrite:
            ModelProcessor.logger.debug(f"{name} exists; skipping.")
            return

        rt.resetMaxFile(rt.Name("NoPrompt"))
        indexes = ModelProcessor.get_indexes_from_name(name, "item_model_ground")
        if len(indexes) == 0:
            ModelProcessor.logger.debug(f"Could not find item: {name}")
            return
        ModelProcessor.logger.info(f"Found {name} indexes: {indexes}")
        ModelProcessor.logger.info("Importing model...")

        for index in indexes:
            ModelProcessor.import_model(index)
            node = rt.GetNodeByName(str(index))
            ModelProcessor.bake_material_to_vtx_color(node)

        ModelProcessor.export_model(name)

    @staticmethod
    def process_all(overwrite: bool = False) -> None:
        items = get_item_list()
        for item in items:
            ModelProcessor.process(item, overwrite)

    @staticmethod
    def launch_gui() -> None:
        gui = ModelProcessor.ModelProcessorGui()
        gui.show()


if __name__ == "__main__":
    ModelProcessor.launch_gui()
    # ModelProcessor.process_all(overwrite=True)
