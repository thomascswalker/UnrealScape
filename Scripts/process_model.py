from pymxs import runtime as rt
import os
import json

project_root = os.path.dirname(os.path.dirname(__file__))
in_model_path = "D:\\osrs\\ModelDumper\\models-out"
out_model_path = os.path.join(project_root, "Assets\\StaticMeshes")
item_data_file = os.path.join(os.path.dirname(__file__), "ItemModelData.json")
with open(item_data_file, "r") as file:
    item_data: dict = json.load(file)


def import_model(index: int) -> None:
    filename = os.path.join(in_model_path, f"{index}.obj")
    if not os.path.exists(filename):
        print(f"File {index} not found.")
    rt.ImportFile(filename, rt.Name("NoPrompt"), using=rt.OBJIMP)


def get_index_from_json(name: str, filt: str) -> int:
    for k, v in item_data.items():
        if v["model_type"] != filt:
            continue
        if name == v["model_name"]:
            return int(v["model_ids"])

    return -1


def gamma_correct(color: rt.Color, gamma: float = 1.0) -> rt.Color:
    r = int(pow((color.r / 255.0), (1.0 / gamma)) * 255.0)
    g = int(pow((color.g / 255.0), (1.0 / gamma)) * 255.0)
    b = int(pow((color.b / 255.0), (1.0 / gamma)) * 255.0)
    return rt.Color(r, g, b)


def get_mtl_color(mtl: rt.Material, face_id: int) -> rt.Color:
    if mtl[face_id]:
        face_mtl = mtl[face_id]
        if face_mtl:
            face_color = face_mtl.DiffuseColor
            return face_color
    return rt.Black


def bake_material_to_vtx_color(node: rt.Node) -> None:
    # Convert to edit poly
    rt.ConvertToPoly(node)
    node.showVertexColors = True

    # Get the object's material
    multi_mtl = node.material
    for face_id in range(node.NumFaces):
        face_color = get_mtl_color(multi_mtl, face_id)
        rt.PolyOp.SetFaceColor(node, 0, face_id + 1, face_color)
    node.material = None


def export_model(node: rt.Node, name: str) -> None:
    new_name = name.title().replace(" ", "")
    filename = os.path.join(out_model_path, f"SM_{new_name}.fbx")
    rt.ExportFile(filename, rt.Name("NoPrompt"), using=rt.FBXEXP)


if __name__ == "__main__":
    keys = ["Copper ore", "Tin ore", "Bronze pickaxe", "Bronze bar", "Bronze dagger"]
    for k in keys:
        rt.resetMaxFile(rt.Name("NoPrompt"))
        index = get_index_from_json(k, "item_model_ground")
        if index == -1:
            print(f"Could not find item: {k}")
            continue
        import_model(index)
        node = rt.GetNodeByName(str(index))
        bake_material_to_vtx_color(node)
        export_model(node, k)
