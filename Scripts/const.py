import csv
import json
import os
from typing import List

ITEM_ROWS = [
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


class ProjectPaths:
    ProjectDir = os.path.dirname(os.path.dirname(__file__))
    ScriptsDir = os.path.join(ProjectDir, "Scripts")
    AssetsDir = os.path.join(ProjectDir, "Assets")
    ContentDir = os.path.join(ProjectDir, "Content")
    StaticMeshesDir = os.path.join(AssetsDir, "StaticMeshes")
    ProjectFile = os.path.join(ProjectDir, "UnrealScape.uproject")
    SourceModelDir = "D:\\osrs\\ModelDumper\\models-out"


class ImportSettings:
    TextureJsonFile = f"{ProjectPaths.ScriptsDir}\\TextureImportSettings.json"
    ItemList = f"{ProjectPaths.ScriptsDir}\\ItemList.json"
    ItemJsonFile = f"{ProjectPaths.ScriptsDir}\\CSVImportSettings.json"
    ItemCsvFile = f"{ProjectPaths.ScriptsDir}\\DT_ItemDefinitions.csv"
    ModelDataFile = f"{ProjectPaths.ScriptsDir}\\ModelData.csv"


class EditorPaths:
    UeEngineDir = "C:\\Program Files\\Epic Games\\UE_5.1\\Engine"
    UeEngineExe = os.path.join(UeEngineDir, "Binaries\\Win64\\UnrealEditor.exe")


def get_item_list() -> List[str]:
    with open(ImportSettings.ItemList, "r") as f:
        allowed_item_list = json.load(f)
    return allowed_item_list


class ModelData(list):
    Id: int = 0
    Name: int = 1
    Type: int = 2
    ModelIds: int = 3


def get_source_model_data() -> ModelData:
    model_data = ModelData()
    with open(ImportSettings.ModelDataFile, "r") as f:
        reader = csv.reader(f, delimiter=",", quotechar='"')
        for row in reader:
            model_data.append(row)
    return model_data


def format_name(in_name: str) -> str:
    chars = [c for c in in_name if c.isalpha() or c.isdigit() or c == " "]
    basename = "".join(chars).rstrip()
    return basename.title().replace(" ", "")
