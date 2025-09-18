# Quick Test Script for Benchmark Setup
# Tests that all benchmark implementations work correctly

param([switch]$Verbose = $false)

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

function Test-Implementation {
    param(
        [string]$Language,
        [string]$Script,
        [string]$TestName
    )
    
    Write-Host "Testing $Language $TestName..." -NoNewline
    
    try {
        switch ($Language.ToLower()) {
            "python" { 
                $result = & python $Script 10 2>&1
            }
            "caesar" { 
                # For now, just check if file exists since we need command line args support
                if (Test-Path $Script) {
                    Write-Host " ✓ (File exists)" -ForegroundColor Green
                    return $true
                } else {
                    Write-Host " ✗ (File missing)" -ForegroundColor Red
                    return $false
                }
            }
            "cpp" { 
                $exeName = [System.IO.Path]::ChangeExtension($Script, ".exe")
                if (Test-Path $exeName) {
                    $result = & $exeName 10 2>&1
                } else {
                    Write-Host " ⚠ (Not compiled)" -ForegroundColor Yellow
                    return $false
                }
            }
        }
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host " ✓" -ForegroundColor Green
            return $true
        } else {
            Write-Host " ✗ (Exit code: $LASTEXITCODE)" -ForegroundColor Red
            if ($Verbose) { Write-Host $result }
            return $false
        }
    }
    catch {
        Write-Host " ✗ (Exception: $_)" -ForegroundColor Red
        return $false
    }
}

Write-Host "Caesar Benchmark Setup Verification" -ForegroundColor Cyan
Write-Host "=" * 40 -ForegroundColor Cyan

$tests = @("fibonacci", "factorial", "prime_check", "loop_intensive", "string_ops")
$languages = @("python", "caesar", "cpp")

$results = @{}
foreach ($lang in $languages) {
    $results[$lang] = @{}
}

foreach ($test in $tests) {
    Write-Host "`nTesting $test implementations:"
    
    foreach ($lang in $languages) {
        $scriptPath = "$ScriptDir\$lang\$test"
        $scriptPath += if ($lang -eq "python") { ".py" } elseif ($lang -eq "caesar") { ".csr" } else { ".cpp" }
        
        $success = Test-Implementation -Language $lang -Script $scriptPath -TestName $test
        $results[$lang][$test] = $success
    }
}

# Summary
Write-Host "`n" -NoNewline
Write-Host "Summary:" -ForegroundColor Yellow
Write-Host "-" * 20 -ForegroundColor Yellow

foreach ($lang in $languages) {
    $successCount = ($results[$lang].Values | Where-Object { $_ -eq $true }).Count
    $totalCount = $results[$lang].Count
    
    $color = if ($successCount -eq $totalCount) { "Green" } elseif ($successCount -gt 0) { "Yellow" } else { "Red" }
    Write-Host "$lang : $successCount/$totalCount tests ready" -ForegroundColor $color
}

Write-Host "`nNext steps:" -ForegroundColor Cyan
Write-Host "1. Build Caesar project if not already done" -ForegroundColor White
Write-Host "2. Add command line argument support to Caesar scripts" -ForegroundColor White  
Write-Host "3. Compile C++ benchmarks with: g++ -O2 -std=c++17 *.cpp" -ForegroundColor White
Write-Host "4. Run full benchmark with: .\run_benchmarks.ps1" -ForegroundColor White