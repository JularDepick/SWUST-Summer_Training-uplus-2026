import os
import re
from collections import defaultdict

DOCSMD_DIR = "./docsmd/"

# parse a single md file, return list of (n, m, value)
def parse_md(filepath):
    records = []
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()
    n = m = 0
    val = None
    metric_name = ""
    for line in lines:
        line = line.strip()
        if line.startswith("# ") and not line.startswith("### "):
            metric_name = line[2:]
        m_n = re.match(r"- n=(\d+),m=(\d+),", line)
        if m_n:
            n = int(m_n.group(1))
            m = int(m_n.group(2))
        # match metric value line: "- metric_name=value"
        if metric_name and line.startswith("- " + metric_name + "="):
            vstr = line[len(metric_name) + 3:]
            # strip units
            vstr = vstr.replace("ms", "").replace("KB", "").replace("kb", "").strip()
            try:
                val = float(vstr)
            except ValueError:
                val = None
            if val is not None and n > 0:
                records.append((n, m, val))
    return metric_name, records

# group by n, compute average
def avg_by_n(records):
    groups = defaultdict(list)
    for n, m, v in records:
        groups[n].append(v)
    result = {}
    for n in sorted(groups.keys()):
        vals = groups[n]
        result[n] = sum(vals) / len(vals)
    return result

# group by m, compute average
def avg_by_m(records):
    groups = defaultdict(list)
    for n, m, v in records:
        groups[m].append(v)
    result = {}
    for m in sorted(groups.keys()):
        vals = groups[m]
        result[m] = sum(vals) / len(vals)
    return result

# metrics that should be grouped by m (edge-only operations)
m_metrics = {"size_bytes", "size_kb"}

def main():
    files = sorted([f for f in os.listdir(DOCSMD_DIR) if f.endswith(".md")])
    for fname in files:
        fpath = os.path.join(DOCSMD_DIR, fname)
        metric_name, records = parse_md(fpath)
        if not records:
            continue
        if metric_name in m_metrics:
            avg = avg_by_m(records)
            key_label = "m"
        else:
            avg = avg_by_n(records)
            key_label = "n"
        print(f"\n=== {metric_name} (avg by {key_label}) ===")
        for k in sorted(avg.keys()):
            print(f"  {key_label}={k:>5d}  avg={avg[k]:.3f}")

if __name__ == "__main__":
    main()
