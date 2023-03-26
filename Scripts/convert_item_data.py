import json
import csv
import os

json_file = os.path.join(os.path.dirname(__file__), "ItemModelData.json")
csv_file = os.path.join(os.path.dirname(__file__), "ItemModelData.csv")

with open(json_file, "r") as f:
    data: dict = json.load(f)
with open(csv_file, "w", newline="") as f:
    writer = csv.writer(
        f,
        delimiter=",",
        quotechar='"',
        quoting=csv.QUOTE_MINIMAL,
    )
    for k, v in data.items():
        writer.writerow(
            [v["model_type_id"], v["model_name"], v["model_type"], v["model_ids"]]
        )
