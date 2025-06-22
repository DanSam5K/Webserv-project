#!/usr/bin/env python3

import os
import sys
import json
import urllib.parse

# Paths
data_file = os.path.join(os.path.dirname(os.path.dirname(__file__)), "data", "data.json")

# Read data
with open(data_file, "r") as f:
    data = json.load(f)

entries = data.get("entries", [])

# Read POST data from argv[1]
post_data_file = sys.argv[1]
with open(post_data_file, "r") as f:
    post_data = f.read()

# Parse POST data
form_data = urllib.parse.parse_qs(post_data)
target_id = form_data.get("id", [""])[0]

# Filter out the entry
data["entries"] = [entry for entry in entries if entry["id"] != target_id]

# Save new data
with open(data_file, "w") as f:
    json.dump(data, f)
