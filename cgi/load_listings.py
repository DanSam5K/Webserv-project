#!/usr/bin/env python3

import os
import json

print("Content-Type: text/html\n")

# Paths
data_file = os.path.join(os.path.dirname(os.path.dirname(__file__)), "data", "data.json")

# Read and parse JSON
with open(data_file, "r") as f:
    data = json.load(f)

entries = sorted(data.get("entries", []), key=lambda x: int(x["id"]))

# Output HTML table rows
for entry in entries:
    print("<tr>")
    print(f"<td>{entry['id']}</td>")
    print(f"<td>{entry['description']}</td>")
    print(f"<td>{entry['price']}</td>")
    print(f"<td>{entry['phone']}</td>")
    print(f"<td><img src='images/{entry['id']}.jpeg' width='100' height='100' alt='car picture' /></td>")
    print("</tr>")
