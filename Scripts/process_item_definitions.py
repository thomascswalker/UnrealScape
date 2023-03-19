import csv
import base64
import json
import os
import subprocess

from osrsbox import items_api

WRITE_ICONS = True
WRITE_CSV = True

UE5_ENGINE_ROOT = "C:\\Program Files\\Epic Games\\UE_5.1\\Engine"
PROJECT_ROOT = os.path.dirname(os.path.dirname(__file__))
EDITOR_CMD = os.path.join(UE5_ENGINE_ROOT, "Binaries\\Win64\\UnrealEditor.exe")
UPROJECT_FILE = os.path.join(PROJECT_ROOT, "UnrealScape.uproject")

allowed_item_list = [
    "ashes",
    "bread",
    "bread dough",
    "bronze axe",
    "bucket",
    "bucket of water",
    "logs",
    "pot",
    "pot of flour",
    "raw shrimps",
    "shrimps",
    "small fishing net",
    "tinderbox",
    "bronze pickaxe",
    "copper ore",
    "tin ore",
]

processed = []

tex_import_json = os.path.join(PROJECT_ROOT, "Scripts\\TextureImportSettings.json")
csv_import_json = os.path.join(PROJECT_ROOT, "Scripts\\CSVImportSettings.json")
out_item_definition_csv = os.path.join(
    os.path.dirname(__file__), "DT_ItemDefinitions.csv"
)

item_rows = [
    "---",
    "Name",
    "Mesh",
    "Id",
    "bEquipable",
    "EquipType",
    "bStackable",
    "Examine",
    "Icon",
    "Options",
]

if __name__ == "__main__":
    # Load all items
    all_db_items = items_api.load()

    with open(out_item_definition_csv, mode="w", newline="") as items_out_fi:
        # Create csv writer
        items_writer = csv.writer(
            items_out_fi,
            delimiter=",",
            quotechar='"',
            quoting=csv.QUOTE_ALL,
        )

        # Write headers
        items_writer.writerow(item_rows)

        for item in all_db_items:
            # Pass on noted or quest items
            if item.noted or item.quest_item:
                continue

            # Only allow items defined in allowed_item_list
            if item.name.lower() not in allowed_item_list:
                print(f"Skipping: {item.name} ({item.id})")
                continue

            # If an item is a duplicate, skip it
            if item.name in processed:
                print(f"Already processed: {item.name} ({item.id})")
                continue

            print(f"Processing: {item.name} ({item.id})")

            # Write out the texture icon
            basename = "".join(
                [c for c in item.name if c.isalpha() or c.isdigit() or c == " "]
            ).rstrip()
            format_name = basename.title().replace(" ", "")
            filename = f"{PROJECT_ROOT}\\Assets\\Textures\\Items\\T_{format_name}.png"
            with open(filename, "wb") as f:
                f.write(base64.urlsafe_b64decode(bytes(item.icon, "utf-8")))

            # Write the filename to the texture import .json file
            tex_json_data = {}
            with open(tex_import_json) as f:
                tex_json_data = json.load(f)
                tex_json_data["ImportGroups"][0]["FileNames"].append(filename)
            with open(tex_import_json, "w") as f:
                json.dump(tex_json_data, f)

            # Create the struct values
            mesh = f"{PROJECT_ROOT}\\Content\\Meshes\\Items\\SM_{format_name}.uasset"
            mesh_exists = os.path.exists(mesh.lower())
            mesh_uasset = f"/Game/Meshes/Items/SM_{format_name}.SM_{format_name}"
            box_uasset = "/Engine/BasicShapes/Cube.Cube"

            # Write the current item to the .csv
            items_writer.writerow(
                [
                    item.id,  # Row
                    item.name,  # Name
                    mesh_uasset if mesh_exists else box_uasset,  # Mesh
                    item.id,  # Id
                    "True" if item.equipable else "False",  # Equipable
                    "",  # EquipType
                    "True" if item.stackable else "False",  # Stackable
                    item.examine,  # Examine
                    f"/Game/Textures/Items/T_{format_name}.T_{format_name}",  # Icon
                    "",  # Options
                ]
            )

            # Add to the processed list to prevent duplicates
            processed.append(item.name)

    # Import all generated .png files
    args = [
        EDITOR_CMD,
        UPROJECT_FILE,
        "-run=ImportAssetsCommandlet",
        f"-importsettings={tex_import_json}",
    ]
    subprocess.run(args)

    # Write the generated .csv filename to the csv import .json file
    csv_json_data = {}
    with open(csv_import_json) as f:
        csv_json_data = json.load(f)
        csv_json_data["ImportGroups"][0]["FileNames"].append(out_item_definition_csv)
    with open(csv_import_json, "w") as f:
        json.dump(csv_json_data, f)

    # Import .csv to create DT_ItemDefinitions DataTable
    args = [
        EDITOR_CMD,
        UPROJECT_FILE,
        "-run=ImportAssetsCommandlet",
        f"-importsettings={csv_import_json}",
    ]
    subprocess.run(args)
