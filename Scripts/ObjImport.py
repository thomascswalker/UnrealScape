from pymxs import runtime as rt
import os
import json

MODEL_PATH = "D:\\osrs\\ModelDumper\\models-out"
with open(os.path.join(os.path.dirname(__file__), "ItemModelData.json"), 'r') as file:
    ITEM_DATA: dict = json.load(file)

def import_model(index: int) -> None:
    filename = os.path.join(MODEL_PATH, f"{index}.obj")
    if not os.path.exists(filename):
        print(f"File {index} not found.")
    rt.ImportFile(filename, rt.Name("NoPrompt"), using=rt.OBJIMP)

def get_index_from_json(name: str):
    """
    {
        "item_model_ground_0_2595": {
            "model_type": "item_model_ground",
            "model_type_id": 0,
            "model_name": "Dwarf remains",
            "model_ids": 2595
        },
    }
    """
    for k, v in ITEM_DATA.items():
        if name == v["model_name"]:
            return int(v["model_ids"])
    
if __name__ == "__main__":
    keys = ["Shrimps", "Raw shrimps", "Small fishing net", "Bronze axe", "Tinderbox"]
    for k in keys:
        index = get_index_from_json(k)
        import_model(index)