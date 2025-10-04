# Modified script that works inside this environment (falls back to cwd if __file__ not available).
# It will create sample_bits.json and generated_struct.h in the current working directory of the notebook.
import json
import os

try:
    SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
except NameError:
    SCRIPT_DIR = os.getcwd()

def load_json_file(file_name):
    """Load JSON file relative to this script directory."""
    file_path = os.path.join(SCRIPT_DIR, file_name)
    with open(file_path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    return data

def normalize_mapping(mapping, total_bits=32, reserved_name="__reserved"):
    names = [reserved_name] * total_bits
    for k, v in mapping.items():
        try:
            idx = int(k)
        except ValueError:
            continue
        if 0 <= idx < total_bits:
            names[idx] = v if (v is not None and v != "") else reserved_name
    return names

def merge_adjacent(names):
    if not names:
        return []
    merged = []
    cur_name = names[0]
    cur_count = 1
    for n in names[1:]:
        if n == cur_name:
            cur_count += 1
        else:
            merged.append((cur_name, cur_count))
            cur_name = n
            cur_count = 1
    merged.append((cur_name, cur_count))
    return merged

def sanitize_field_name(name, reserved_prefix="rsv"):
    if name.startswith("__reserved"):
        return None
    s = name.strip()
    s = s.replace(" ", "_").replace("-", "_")
    s = "".join(ch if (ch.isalnum() or ch == "_") else "_" for ch in s)
    if not s:
        return None
    if s[0].isdigit():
        s = "_" + s
    return s.lower()

def generate_struct(struct_name, mapping, total_bits=32):
    names = normalize_mapping(mapping, total_bits=total_bits)
    merged = merge_adjacent(names)

    lines = []
    lines.append(f"/* Generated struct for {struct_name} - total {total_bits} bits */")
    lines.append(f"typedef union {{")
    lines.append(f"    uint32_t value;")
    lines.append(f"    struct {{ /* LSB (bit 0) first - implementation-defined layout */")

    reserved_counter = 0
    bit_offset = 0
    for name, width in merged:
        if name.startswith("__reserved"):
            field_name = f"reserved_{reserved_counter}"
            reserved_counter += 1
        else:
            field_name = sanitize_field_name(name)
            if field_name is None:
                field_name = f"reserved_{reserved_counter}"
                reserved_counter += 1
        lines.append(f"        unsigned {field_name} : {width};  /* bits {bit_offset}..{bit_offset+width-1} */")
        bit_offset += width

    lines.append(f"    }} bits;")
    lines.append(f"}} {struct_name}_t;")

    return "\n".join(lines)

# Sample JSON to demonstrate behavior
sample_json = {
    "0": "FLAG_A",
    "1": "FLAG_A",
    "2": "MODE",
    "3": "MODE",
    "4": "MODE",
    "5": "",
    "6": "ERROR",
    "7": "ERROR",
    "8": "COUNT",
    "9": "COUNT",
    "10": "COUNT",
    "11": "COUNT",
    "12": "COUNT",
    "13": "FLAG_B",
    "14": "FLAG_B",
    "15": "FLAG_B",
    "16": "FIELD_X",
    "17": "FIELD_X",
    "18": "FIELD_X",
    "19": "FIELD_X",
    "20": "FIELD_X",
    "21": "FIELD_X",
    "22": "FIELD_X",
    "23": "FIELD_X",
    "24": "FIELD_X",
    "25": "FIELD_X",
    "26": "FIELD_X",
    "27": "FIELD_X",
    "28": "FIELD_X",
    "29": "FIELD_X",
    "30": "FIELD_X",
    "31": "FIELD_X"
}

sample_path = os.path.join(SCRIPT_DIR, "sample_bits.json")
with open(sample_path, "w", encoding="utf-8") as f:
    json.dump(sample_json, f, indent=4)

mapping = load_json_file("sample_bits.json")
struct_text = generate_struct("control_reg", mapping, total_bits=32)

out_path = os.path.join(SCRIPT_DIR, "generated_struct.h")
with open(out_path, "w", encoding="utf-8") as f:
    f.write("#include <stdint.h>\n\n")
    f.write(struct_text)
    f.write("\n")

print("Sample JSON written to:", sample_path)
print("Generated header written to:", out_path)
print("\n--- Generated struct preview ---\n")
print(struct_text)
