import os
import re

def analyze_cpp_source(filepath):
    """
    Analyzes the C++ source file to heuristically determine feature implementation.
    Returns a dictionary of feature statuses.
    """
    statuses = {
        "data pipeline": "Not Started",
        "dashboard": "Not Started",
        "rules engine": "Not Started",
        "notification channels": "Not Started"
    }

    try:
        with open(filepath, 'r') as f:
            content = f.read()

        if 'LogParser::parseLogFile' in content:
            statuses["data pipeline"] = "In Progress (from file)"

        if 'checkAlerts' in content and 'AlertThresholds thresholds' in content:
            statuses["rules engine"] = "In Progress (hardcoded)"

        if 'std::cout << "--- Monitoring Metrics ---"' in content:
            statuses["dashboard"] = "Not Started (prints to console)"

    except FileNotFoundError:
        print(f"FATAL: Source code file not found at {filepath}")
        exit(1)

    return statuses

def parse_plan_and_print_report(plan_filepath, feature_statuses):
    """
    Parses the plan.md file, combines with feature status, and prints the report.
    """
    print("\n--- Development Plan Progress ---")

    task_to_feature_key = {
        "data pipeline": "data pipeline",
        "dashboard": "dashboard",
        "rules engine": "rules engine",
        "notification channels": "notification channels"
    }

    try:
        with open(plan_filepath, 'r') as f:
            lines = f.readlines()

        current_phase = ""
        for line in lines:
            line = line.strip()
            if not line:
                continue

            if line.startswith("###"):
                current_phase = line
                print(f"\n{current_phase}")
                continue

            match = re.match(r"^\*   \*\*(.+?)\*\*.*", line)
            if match:
                task_name = match.group(1)

                status_symbol = "[ ]"
                status_text = "Not Started"

                if "Phase 1" in current_phase:
                    status_symbol = "[x]"
                    status_text = "Complete (Documentation)"
                else:
                    for task_keyword, feature_key in task_to_feature_key.items():
                        if task_keyword in task_name.lower():
                            feature_status = feature_statuses.get(feature_key, "Not Started")
                            if "Not Started" not in feature_status:
                                status_symbol = "[x]"
                                status_text = feature_status
                            else:
                                status_symbol = "[ ]"
                                status_text = feature_status
                            break

                print(f"  {status_symbol} {task_name} ({status_text})")

    except FileNotFoundError:
        print(f"FATAL: Plan file not found at {plan_filepath}")
        exit(1)

def main():
    """
    Main function to run the monitoring tool.
    """
    print("--- QuantaPulsa Development Progress Monitor ---")

    cpp_source_path = "src/QuantaPulsa.cpp"
    plan_path = "docs/plan.md"

    feature_statuses = analyze_cpp_source(cpp_source_path)
    parse_plan_and_print_report(plan_path, feature_statuses)

    print("\n--- End of Report ---")

if __name__ == "__main__":
    main()
