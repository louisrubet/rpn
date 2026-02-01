// Integration tests using markdown test files

use rpn_rust::test_framework;
use std::path::PathBuf;

fn test_dir() -> PathBuf {
    PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("test")
}

#[test]
fn test_005_test_framework() {
    let path = test_dir().join("005-test-framework.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!(
        "005-test-framework.md: {} passed, {} failed",
        passed, failed
    );
    assert_eq!(failed, 0, "Some tests failed");
}

#[test]
fn test_010_mode() {
    let path = test_dir().join("010-mode.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!("010-mode.md: {} passed, {} failed", passed, failed);
    assert_eq!(failed, 0, "Some tests failed");
}

#[test]
fn test_020_general() {
    let path = test_dir().join("020-general.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!("020-general.md: {} passed, {} failed", passed, failed);
    assert_eq!(failed, 0, "Some tests failed");
}

#[test]
fn test_040_stack() {
    let path = test_dir().join("040-stack.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!("040-stack.md: {} passed, {} failed", passed, failed);
    assert_eq!(failed, 0, "Some tests failed");
}

#[test]
fn test_070_test() {
    let path = test_dir().join("070-test.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!("070-test.md: {} passed, {} failed", passed, failed);
    assert_eq!(failed, 0, "Some tests failed");
}

#[test]
fn test_080_store() {
    let path = test_dir().join("080-store.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!("080-store.md: {} passed, {} failed", passed, failed);
    assert_eq!(failed, 0, "Some tests failed");
}

#[test]
fn test_120_trig() {
    let path = test_dir().join("120-trig.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!("120-trig.md: {} passed, {} failed", passed, failed);
    assert_eq!(failed, 0, "Some tests failed");
}

#[test]
fn test_130_logs() {
    let path = test_dir().join("130-logs.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!("130-logs.md: {} passed, {} failed", passed, failed);
    assert_eq!(failed, 0, "Some tests failed");
}

#[test]
fn test_090_program() {
    let path = test_dir().join("090-program.md");
    let (_test_file, results) =
        test_framework::run_test_file(&path).expect("Failed to run test file");

    let mut passed = 0;
    let mut failed = 0;

    for result in &results {
        if result.passed {
            passed += 1;
        } else {
            failed += 1;
            eprintln!("FAILED: {} - {}", result.name, result.message);
        }
    }

    println!("090-program.md: {} passed, {} failed", passed, failed);
    assert_eq!(failed, 0, "Some tests failed");
}
