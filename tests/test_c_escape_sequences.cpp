/**
 * @file test_c_escape_sequences.cpp
 * @brief Comprehensive tests for escape sequences in C code generation
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>

bool compileAndTest(const std::string& caesar_code, const std::string& test_name, const std::string& expected_output) {
    std::string caesar_file = "/tmp/test_escape_" + test_name + ".csr";
    std::string c_file = "/tmp/test_escape_" + test_name + ".c";
    std::string exe_file = "/tmp/test_escape_" + test_name;
    
    // Write Caesar code
    std::ofstream out(caesar_file);
    out << caesar_code;
    out.close();
    
    // Compile to C
    std::string compile_cmd = "../src/caesar --c " + caesar_file + " -o " + c_file + " 2>&1";
    if (system(compile_cmd.c_str()) != 0) {
        std::cerr << "Failed to compile Caesar to C: " << test_name << std::endl;
        system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
        return false;
    }
    
    // Compile C with GCC
    std::string gcc_cmd = "gcc -std=c11 " + c_file + " -o " + exe_file + " -lm 2>&1";
    if (system(gcc_cmd.c_str()) != 0) {
        std::cerr << "Failed to compile C with GCC: " << test_name << std::endl;
        system(("rm -f " + caesar_file + " " + c_file + " " + exe_file).c_str());
        return false;
    }
    
    // Execute and capture output
    std::string run_cmd = exe_file + " > /tmp/test_escape_output_" + test_name + ".txt 2>&1";
    system(run_cmd.c_str());
    
    // Read output
    std::ifstream result_file("/tmp/test_escape_output_" + test_name + ".txt");
    std::string actual_output((std::istreambuf_iterator<char>(result_file)),
                              std::istreambuf_iterator<char>());
    result_file.close();
    
    // Compare output
    bool passed = (actual_output == expected_output);
    
    // Clean up
    system(("rm -f " + caesar_file + " " + c_file + " " + exe_file + " /tmp/test_escape_output_" + test_name + ".txt").c_str());
    
    if (passed) {
        std::cout << "PASS: " << test_name << std::endl;
    } else {
        std::cerr << "FAIL: " << test_name << std::endl;
        std::cerr << "  Expected: " << expected_output << std::endl;
        std::cerr << "  Got:      " << actual_output << std::endl;
    }
    
    return passed;
}

// Test 1: Newline escape
bool test_newline() {
    return compileAndTest(
        "text = \"Hello\\nWorld\"\nprint(text)\n",
        "newline",
        "Hello\nWorld\n"
    );
}

// Test 2: Tab escape
bool test_tab() {
    return compileAndTest(
        "text = \"Column1\\tColumn2\"\nprint(text)\n",
        "tab",
        "Column1\tColumn2\n"
    );
}

// Test 3: Carriage return escape
bool test_carriage_return() {
    return compileAndTest(
        "text = \"Line1\\rLine2\"\nprint(text)\n",
        "carriage_return",
        "Line1\rLine2\n"
    );
}

// Test 4: Backslash escape
bool test_backslash() {
    return compileAndTest(
        "text = \"Path\\\\to\\\\file\"\nprint(text)\n",
        "backslash",
        "Path\\to\\file\n"
    );
}

// Test 5: Double quote escape
bool test_double_quote() {
    return compileAndTest(
        "text = \"Say \\\"hello\\\"\"\nprint(text)\n",
        "double_quote",
        "Say \"hello\"\n"
    );
}

// Test 6: Single quote escape
bool test_single_quote() {
    return compileAndTest(
        "text = \"It\\'s working\"\nprint(text)\n",
        "single_quote",
        "It's working\n"
    );
}

// Test 7: Null character escape
bool test_null_char() {
    // Note: This will terminate the string at the null character
    return compileAndTest(
        "text = \"Before\\0After\"\nprint(text)\n",
        "null_char",
        "Before\n"  // Only prints up to null
    );
}

// Test 8: Multiple escapes in one string
bool test_multiple_escapes() {
    return compileAndTest(
        "text = \"Line1\\nTab:\\tValue\\nQuote: \\\"text\\\"\"\nprint(text)\n",
        "multiple_escapes",
        "Line1\nTab:\tValue\nQuote: \"text\"\n"
    );
}

// Test 9: Empty string
bool test_empty_string() {
    return compileAndTest(
        "text = \"\"\nprint(text)\n",
        "empty_string",
        "\n"
    );
}

// Test 10: String with only escape
bool test_only_escape() {
    return compileAndTest(
        "text = \"\\n\"\nprint(text)\n",
        "only_escape",
        "\n\n"  // One from the string, one from print
    );
}

// Test 11: Consecutive escapes
bool test_consecutive_escapes() {
    return compileAndTest(
        "text = \"\\n\\n\\n\"\nprint(text)\n",
        "consecutive_escapes",
        "\n\n\n\n"  // Three from string, one from print
    );
}

// Test 12: Escape at start
bool test_escape_at_start() {
    return compileAndTest(
        "text = \"\\tIndented\"\nprint(text)\n",
        "escape_at_start",
        "\tIndented\n"
    );
}

// Test 13: Escape at end
bool test_escape_at_end() {
    return compileAndTest(
        "text = \"Text\\n\"\nprint(text)\n",
        "escape_at_end",
        "Text\n\n"
    );
}

// Test 14: Mixed content
bool test_mixed_content() {
    return compileAndTest(
        "text = \"Name:\\tJohn\\nAge:\\t25\\nCity:\\tNY\"\nprint(text)\n",
        "mixed_content",
        "Name:\tJohn\nAge:\t25\nCity:\tNY\n"
    );
}

// Test 15: String in expression
bool test_escape_in_expression() {
    return compileAndTest(
        "a = \"Hello\\n\"\nb = \"World\"\nresult = a\nprint(result)\n",
        "escape_in_expression",
        "Hello\n\n"
    );
}

// Test 16: String in loop
bool test_escape_in_loop() {
    return compileAndTest(
        "for i in range(2):\n    print(\"Line\\n\")\n",
        "escape_in_loop",
        "Line\n\nLine\n\n"
    );
}

// Test 17: Backslash at end (should just be backslash)
bool test_backslash_literal() {
    return compileAndTest(
        "text = \"\\\\\"\nprint(text)\n",
        "backslash_literal",
        "\\\n"
    );
}

// Test 18: Unicode-like sequences (not supported, treated as literal)
bool test_unknown_escape() {
    // Unknown escape sequences: backslash is consumed, character kept
    // Lexer treats \x as just 'x' (unknown escape)
    return compileAndTest(
        "text = \"\\x41\"\nprint(text)\n",
        "unknown_escape",
        "x41\n"  // Backslash consumed by lexer, 'x' and '41' remain
    );
}

// Test 19: Long string with many escapes
bool test_long_string() {
    return compileAndTest(
        "text = \"Line1\\nLine2\\tTabbed\\nLine3\\\\Backslash\\nLine4\\\"Quote\\\"\"\nprint(text)\n",
        "long_string",
        "Line1\nLine2\tTabbed\nLine3\\Backslash\nLine4\"Quote\"\n"
    );
}

// Test 20: String comparison with escapes
bool test_escape_in_comparison() {
    return compileAndTest(
        "a = \"Hello\\nWorld\"\nb = \"Hello\\nWorld\"\nif a == b:\n    print(\"Match\")\n",
        "escape_in_comparison",
        "Match\n"
    );
}

int main() {
    int passed = 0;
    int total = 20;
    
    std::cout << "Running escape sequence tests..." << std::endl;
    std::cout << "========================================" << std::endl;
    
    if (test_newline()) passed++;
    if (test_tab()) passed++;
    if (test_carriage_return()) passed++;
    if (test_backslash()) passed++;
    if (test_double_quote()) passed++;
    if (test_single_quote()) passed++;
    if (test_null_char()) passed++;
    if (test_multiple_escapes()) passed++;
    if (test_empty_string()) passed++;
    if (test_only_escape()) passed++;
    if (test_consecutive_escapes()) passed++;
    if (test_escape_at_start()) passed++;
    if (test_escape_at_end()) passed++;
    if (test_mixed_content()) passed++;
    if (test_escape_in_expression()) passed++;
    if (test_escape_in_loop()) passed++;
    if (test_backslash_literal()) passed++;
    if (test_unknown_escape()) passed++;
    if (test_long_string()) passed++;
    if (test_escape_in_comparison()) passed++;
    
    std::cout << "========================================" << std::endl;
    std::cout << "Test Results: " << passed << "/" << total << " passed" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return (passed == total) ? 0 : 1;
}
