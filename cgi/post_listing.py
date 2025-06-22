#!/usr/bin/env python3

import os
import sys
import json
import cgi
import urllib.parse

# Paths
data_file = os.path.join(os.path.dirname(os.path.dirname(__file__)), "data", "data.json")

# Read JSON data
with open(data_file, "r") as f:
    data = json.load(f)

entries = data.get("entries", [])

# Read post data from argv[1]
post_data_file = sys.argv[1]
with open(post_data_file, "r") as f:
    post_data = f.read()

# Parse the POST data
form_data = urllib.parse.parse_qs(post_data)
description = form_data.get("description", [""])[0]
price = form_data.get("price", [""])[0]
phone = form_data.get("phone", [""])[0]

# Find unused ID
used_ids = sorted([int(entry["id"]) for entry in entries])
new_id = next(i for i in range(1, len(used_ids) + 2) if i not in used_ids)

# Echo the ID for use by the file upload
print(new_id)

# Add entry
entries.append({
    "id": str(new_id),
    "description": description,
    "price": price,
    "phone": phone
})

# Save updated data
data["entries"] = entries
with open(data_file, "w") as f:
    json.dump(data, f)
