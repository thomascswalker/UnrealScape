import csv
import base64
import json
import logging
import os
import subprocess
from typing import List

from const import (
    ProjectPaths,
    EditorPaths,
    ImportSettings,
    ITEM_ROWS,
    get_item_list,
    format_name,
)
from osrsbox import items_api
from osrsbox.items_api.all_items import AllItems, ItemProperties

from logger import get_logger


class ItemDefProcessor:
    logger = get_logger(__name__, logging.INFO)

    allow_list: List[str]
    processed: List[str]
    all_items: AllItems = items_api.load()
    num_errors: int = 0

    texture_settings: dict = {
        "ImportGroups": [
            {
                "bReplaceExisting": "true",
                "bSkipReadOnly": "true",
                "DestinationPath": "/Game/Textures/Items",
                "FactoryName": "TextureFactory",
                "FileNames": [],
                "GroupName": "Test",
            }
        ]
    }

    item_def_settings: dict = {
        "ImportGroups": [
            {
                "bReplaceExisting": "true",
                "bSkipReadOnly": "true",
                "DestinationPath": "/Game/Blueprints/Items",
                "FactoryName": "CSVImportFactory",
                "FileNames": [],
                "GroupName": "Test",
                "ImportSettings": {
                    "ImportAs": "DataTable",
                    "ImportRowStruct": "ItemDef",
                    "ImportType": "ECSV_DataTable",
                },
            }
        ]
    }

    def __init__(self, allow_list: List[str] = []) -> None:
        self.processed = []
        self.allow_list = allow_list

    def is_allowed(self, item: ItemProperties) -> bool:
        basename = format_name(item.name)
        is_processed = basename in self.processed
        is_allowed = basename in [format_name(n) for n in self.allow_list]
        return not is_processed and is_allowed

    def write_texture(self, item: ItemProperties, basename: str) -> str:
        # Build the texture subpath
        tex_subpath = f"Textures\\Items\\T_{basename}.png"

        # Build the texture output path
        tex_outpath = os.path.join(ProjectPaths.AssetsDir, tex_subpath)

        self.logger.info(f"Writing texture: {tex_outpath}.")

        # Write the texture from bytes to a PNG file (no PNG header)
        with open(tex_outpath, "wb") as f:
            f.write(base64.urlsafe_b64decode(bytes(item.icon, "utf-8")))

        self.logger.success(f"Texture written for {basename}: {tex_outpath}")

        return tex_outpath

    def write_csv(self) -> None:
        with open(ImportSettings.ItemCsvFile, mode="w", newline="") as out_csv_file:
            # Create csv writer
            writer = csv.writer(
                out_csv_file,
                delimiter=",",
                quotechar='"',
                quoting=csv.QUOTE_ALL,
            )

            # Write headers
            writer.writerow(ITEM_ROWS)

            for item in self.all_items:
                self.logger.debug(f"Processing: {item.name}")
                basename = format_name(item.name)

                # Determine if we've processed this item or if it's not allowed
                # If it doesn't pass the check, we'll skip it
                if not self.is_allowed(item):
                    self.logger.debug(f"Already processed or not allowed: {item.name}")
                    continue

                # Write the texture to a file
                tex_file = self.write_texture(item, basename)

                # Append the new texture filename to the list of textures to import
                group = self.texture_settings["ImportGroups"][0]
                group["FileNames"].append(tex_file)

                # Create the mesh paths
                out_model_subpath = f"Meshes\\Items\\SM_{basename}.uasset"
                mesh = os.path.join(ProjectPaths.ContentDir, out_model_subpath)
                mesh_exists = os.path.exists(mesh)
                mesh_uasset = f"/Game/Meshes/Items/SM_{basename}.SM_{basename}"
                box_uasset = "/Engine/BasicShapes/Cube.Cube"

                if not mesh_exists:
                    msg = f"Mesh not found for {basename}, defaulting to Cube."
                    self.logger.warning(msg)

                # Write the current item to the .csv
                row = [
                    item.id,  # Row
                    item.name,  # Name
                    mesh_uasset if mesh_exists else box_uasset,  # Mesh
                    item.id,  # Id
                    "True" if item.equipable else "False",  # Equipable
                    "",  # EquipType
                    "True" if item.stackable else "False",  # Stackable
                    item.examine,  # Examine
                    f"/Game/Textures/Items/T_{basename}.T_{basename}",  # Icon
                    "",  # Options
                ]
                writer.writerow(row)
                self.logger.success(f"Row written for {item.name}")
                self.logger.info(json.dumps(row, indent=4))

                # Mark that we've processed this item to prevent duplicates
                self.logger.success(f"Finished processing: {basename}")
                self.processed.append(basename)

        # Add the final generated .csv file to the import settings for Item defs
        group = self.item_def_settings["ImportGroups"][0]
        group["FileNames"].append(ImportSettings.ItemCsvFile)

    def write_json(self) -> None:
        self.logger.info(f"Writing {ImportSettings.TextureJsonFile}.")
        with open(ImportSettings.TextureJsonFile, "w") as f:
            json.dump(self.texture_settings, f)

        self.logger.info(f"Writing {ImportSettings.ItemJsonFile}.")
        with open(ImportSettings.ItemJsonFile, "w") as f:
            json.dump(self.item_def_settings, f)

    def import_textures(self) -> None:
        self.logger.info("Importing textures.")
        # Import all generated .png files
        args = [
            EditorPaths.UeEngineExe,
            ProjectPaths.ProjectFile,
            "-run=ImportAssetsCommandlet",
            f"-importsettings={ImportSettings.TextureJsonFile}",
        ]

        p = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        try:
            p.check_returncode()
            self.logger.success("Successfully imported item definitions.")
        except subprocess.CalledProcessError:
            self.num_errors += 1
            self.logger.error("An error occurred importing item definitions.")
            self.logger.error(p.stderr)

    def import_item_definitions(self) -> None:
        self.logger.info("Importing item definitions.")
        # Import .csv to create DT_ItemDefinitions DataTable
        args = [
            EditorPaths.UeEngineExe,
            ProjectPaths.ProjectFile,
            "-run=ImportAssetsCommandlet",
            f"-importsettings={ImportSettings.ItemJsonFile}",
        ]
        p = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        try:
            p.check_returncode()
            self.logger.success("Successfully imported item definitions.")
        except subprocess.CalledProcessError:
            self.num_errors += 1
            self.logger.error("An error occurred importing item definitions.")
            self.logger.error(p.stderr)

    def process(self) -> None:
        self.logger.info("Starting processing")
        self.write_csv()
        self.write_json()
        self.import_textures()
        self.import_item_definitions()

        self.logger.success(f"Finished processing items ({self.num_errors} errors)")


if __name__ == "__main__":
    items = get_item_list()
    item_processor = ItemDefProcessor(items)
    item_processor.process()
