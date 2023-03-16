import csv
import base64
import os

from osrsbox import items_api

WRITE_ICONS = True
WRITE_CSV = True


if __name__ == "__main__":
    # Load all items
    all_db_items = items_api.load()
    for item in all_db_items:
        if item.members or item.quest_item:
            continue
        print(item.name)
    # with open("items.csv", mode="w", newline="") as items_out_fi:

    # basename = "".join(
    #     [c for c in item.name if c.isalpha() or c.isdigit() or c == " "]
    # ).rstrip()
    # format_name = basename.title().replace(" ", "")
    # root = os.path.dirname(os.path.dirname(__file__))
    # filename = f"{root}\\Assets\\Textures\\Items\\T_{format_name}.png"
    # with open(filename, "wb") as f:
    #     f.write(base64.urlsafe_b64decode(bytes(item.icon, "utf-8")))

    # items_writer = csv.writer(
    #     items_out_fi,
    #     delimiter=",",
    #     quotechar='"',
    #     quoting=csv.QUOTE_MINIMAL,
    # )
    # items_writer.writerow(
    #     [
    #         "---",
    #         "Name",
    #         "Mesh",
    #         "Id",
    #         "bEquipable",
    #         "EquipType",
    #         "bStackable",
    #         "Examine",
    #         "Icon",
    #         "Options",
    #     ]
    # )
    # # Icon /Script/Engine.Texture2D'"/Game/Textures/Items/T_RawShrimps.T_RawShrimps"'
    # items_writer.writerow(
    #     [
    #         item.id,  # Row
    #         item.name,  # Name
    #         "",  # Mesh
    #         item.id,  # Id
    #         "True" if item.equipable else "False",  # Equipable
    #         "",  # EquipType
    #         "True" if item.stackable else "False",  # Stackable
    #         item.examine,  # Examine
    #         f"/Game/Textures/Items/T_{format_name}.T_{format_name}"
    #         "",  # Options
    #     ]
    # )
