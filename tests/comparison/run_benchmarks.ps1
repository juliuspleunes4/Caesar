# Caesar Performance Benchmark Suite
# Compares performance between Python, Caesar, and C++

param(
    [string]$Category = "all",
    [int]$Iterations = 1000,
    [switch]$Verbose = $false,
    [switch]$OutputCsv = $false
)

# Configuration
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Split-Path -Parent (Split-Path -Parent $ScriptDir)
$CaesarExe = "$ProjectRoot\build\src\caesar.exe"
$PythonExe = "python"

# Benchmark categories
$Categories = @{
    "fibonacci" = @{
        "name" = "Fibonacci Sequence"
        "description" = "Recursive and iterative fibonacci computation"
        "scales" = @(20, 25, 30, 35)
    }
    "factorial" = @{
        "name" = "Factorial Computation"
        "description" = "Recursive factorial calculation"
        "scales" = @(10, 15, 20)
    }
    "prime_check" = @{
        "name" = "Prime Number Checking"
        "description" = "Prime checking algorithm efficiency"
        "scales" = @(1000, 10000, 100000)
    }
    "loop_intensive" = @{
        "name" = "Loop Performance"
        "description" = "Tight loop computation performance"
        "scales" = @(100000, 1000000, 10000000)
    }
    "string_ops" = @{
        "name" = "String Operations"
        "description" = "String manipulation and processing"
        "scales" = @(1000, 10000, 100000)
    }
}

# Results storage
$Results = @()

# Color output functions
function Write-Header($text) {
    Write-Host "`n" -NoNewline
    Write-Host "=" * 60 -ForegroundColor Cyan
    Write-Host $text -ForegroundColor Yellow
    Write-Host "=" * 60 -ForegroundColor Cyan
}

function Write-SubHeader($text) {
    Write-Host "`n$text" -ForegroundColor Green
    Write-Host "-" * $text.Length -ForegroundColor Green
}

function Write-Success($text) {
    Write-Host $text -ForegroundColor Green
}

function Write-Warning($text) {
    Write-Host $text -ForegroundColor Yellow
}

function Write-Error($text) {
    Write-Host $text -ForegroundColor Red
}

# Benchmark execution function
function Invoke-Benchmark {
    param(
        [string]$Language,
        [string]$Script,
        [string]$Arguments,
        [int]$Iterations,
        [string]$TestName
    )
    
    if (!(Test-Path $Script)) {
        Write-Warning "Script not found: $Script"
        return $null
    }
    
    $times = @()
    $executable = ""
    $scriptArgs = ""
    
    switch ($Language.ToLower()) {
        "python" { 
            $executable = $PythonExe
            $scriptArgs = "$Script $Arguments"
        }
        "caesar" { 
            $executable = $CaesarExe
            $scriptArgs = "--interpret $Script $Arguments"
        }
        "cpp" { 
            $executable = $Script  # Pre-compiled executable
            $scriptArgs = $Arguments
        }
    }
    
    Write-Host "  Running $Language ($TestName)..." -NoNewline
    
    for ($i = 1; $i -le $Iterations; $i++) {
        try {
            $stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
            
            if ($Language -eq "cpp") {
                if ($Arguments) {
                    & $executable $Arguments.Split(' ') 2>&1 | Out-Null
                } else {
                    & $executable 2>&1 | Out-Null
                }
            } else {
                & $executable $scriptArgs.Split(' ') 2>&1 | Out-Null
            }
            
            $stopwatch.Stop()
            $times += $stopwatch.Elapsed.TotalMilliseconds
            
            if ($Verbose -and ($i % 100 -eq 0)) {
                Write-Host "." -NoNewline
            }
        }
        catch {
            Write-Error " FAILED"
            Write-Error "Error executing $Language benchmark: $_"
            return $null
        }
    }
    
    $avgTime = ($times | Measure-Object -Average).Average
    $stdDev = [Math]::Sqrt((($times | ForEach-Object { [Math]::Pow($_ - $avgTime, 2) }) | Measure-Object -Average).Average)
    
    Write-Success " ✓ Avg: $([Math]::Round($avgTime, 3))ms"
    
    return @{
        Language = $Language
        TestName = $TestName
        AverageTime = $avgTime
        StandardDeviation = $stdDev
        MinTime = ($times | Measure-Object -Minimum).Minimum
        MaxTime = ($times | Measure-Object -Maximum).Maximum
        Iterations = $Iterations
    }
}

# Build C++ benchmarks
function Build-CppBenchmarks {
    Write-SubHeader "Building C++ Benchmarks"
    
    $cppFiles = Get-ChildItem "$ScriptDir\cpp\*.cpp" -ErrorAction SilentlyContinue
    
    foreach ($file in $cppFiles) {
        $exeName = $file.BaseName + ".exe"
        $exePath = "$ScriptDir\cpp\$exeName"
        
        Write-Host "  Building $($file.Name)..." -NoNewline
        
        try {
            $compileResult = & g++ -O2 -std=c++17 $file.FullName -o $exePath 2>&1
            if ($LASTEXITCODE -eq 0) {
                Write-Success " ✓"
            } else {
                Write-Error " FAILED"
                Write-Error $compileResult
            }
        }
        catch {
            Write-Error " FAILED"
            Write-Error "G++ not found or compilation failed: $_"
        }
    }
}

# Verify prerequisites
function Test-Prerequisites {
    Write-SubHeader "Checking Prerequisites"
    
    $allGood = $true
    
    # Check Caesar executable
    if (Test-Path $CaesarExe) {
        Write-Success "  ✓ Caesar executable found"
    } else {
        Write-Error "  ✗ Caesar executable not found at: $CaesarExe"
        Write-Error "    Please build Caesar first: cd build && make"
        $allGood = $false
    }
    
    # Check Python
    try {
        $pythonVersion = & python --version 2>&1
        Write-Success "  ✓ Python found: $pythonVersion"
    }
    catch {
        Write-Error "  ✗ Python not found"
        $allGood = $false
    }
    
    # Check G++
    try {
        $gppVersion = & g++ --version 2>&1 | Select-Object -First 1
        Write-Success "  ✓ G++ found: $gppVersion"
    }
    catch {
        Write-Warning "  ⚠ G++ not found - C++ benchmarks will be skipped"
    }
    
    return $allGood
}

# Run specific benchmark category
function Invoke-CategoryBenchmark {
    param($CategoryName, $CategoryInfo)
    
    Write-SubHeader $CategoryInfo.name
    Write-Host $CategoryInfo.description
    
    foreach ($scale in $CategoryInfo.scales) {
        Write-Host "`n  Scale: $scale"
        
        $testResults = @()
        
        # Python benchmark
        $pythonScript = "$ScriptDir\python\$CategoryName.py"
        $pythonResult = Invoke-Benchmark -Language "Python" -Script $pythonScript -Arguments $scale -Iterations $Iterations -TestName "Scale_$scale"
        if ($pythonResult) { $testResults += $pythonResult }
        
        # Caesar benchmark
        $caesarScript = "$ScriptDir\caesar\$CategoryName.csr"
        $caesarResult = Invoke-Benchmark -Language "Caesar" -Script $caesarScript -Arguments $scale -Iterations $Iterations -TestName "Scale_$scale"
        if ($caesarResult) { $testResults += $caesarResult }
        
        # C++ benchmark
        $cppExe = "$ScriptDir\cpp\$CategoryName.exe"
        $cppResult = Invoke-Benchmark -Language "C++" -Script $cppExe -Arguments $scale -Iterations $Iterations -TestName "Scale_$scale"
        if ($cppResult) { $testResults += $cppResult }
        
        # Calculate relative performance
        if ($testResults.Count -gt 1) {
            $baseline = ($testResults | Sort-Object AverageTime)[0]
            
            Write-Host "`n  Performance Comparison (Scale: $scale):"
            foreach ($result in $testResults | Sort-Object AverageTime) {
                $speedup = [Math]::Round($baseline.AverageTime / $result.AverageTime, 2)
                $relativeSpeed = if ($speedup -ge 1) { "${speedup}x faster" } else { "$([Math]::Round(1/$speedup, 2))x slower" }
                
                if ($result.Language -eq $baseline.Language) {
                    Write-Success "    $($result.Language): $([Math]::Round($result.AverageTime, 3))ms (baseline)"
                } else {
                    Write-Host "    $($result.Language): $([Math]::Round($result.AverageTime, 3))ms ($relativeSpeed than $($baseline.Language))"
                }
            }
        }
        
        $Results += $testResults
    }
}

# Generate summary report
function Write-SummaryReport {
    Write-Header "Benchmark Summary"
    
    if ($Results.Count -eq 0) {
        Write-Warning "No benchmark results to summarize."
        return
    }
    
    # Group by language and calculate averages
    $languageStats = $Results | Group-Object Language | ForEach-Object {
        $avgTime = ($_.Group | Measure-Object AverageTime -Average).Average
        $testCount = $_.Count
        
        @{
            Language = $_.Name
            AverageTime = $avgTime
            TestCount = $testCount
        }
    }
    
    Write-Host "`nOverall Performance Ranking:"
    $ranking = $languageStats | Sort-Object AverageTime
    for ($i = 0; $i -lt $ranking.Count; $i++) {
        $lang = $ranking[$i]
        $position = $i + 1
        Write-Host "  $position. $($lang.Language): $([Math]::Round($lang.AverageTime, 3))ms average ($($lang.TestCount) tests)"
    }
    
    # Calculate speed ratios
    if ($ranking.Count -gt 1) {
        Write-Host "`nSpeed Comparisons:"
        $fastest = $ranking[0]
        
        foreach ($lang in $ranking[1..($ranking.Count-1)]) {
            $ratio = [Math]::Round($lang.AverageTime / $fastest.AverageTime, 2)
            Write-Host "  $($fastest.Language) is ${ratio}x faster than $($lang.Language)"
        }
    }
}

# Export results to CSV
function Export-ResultsToCsv {
    if ($Results.Count -eq 0) {
        return
    }
    
    $csvPath = "$ScriptDir\benchmark_results.csv"
    $Results | Export-Csv -Path $csvPath -NoTypeInformation
    Write-Success "`nResults exported to: $csvPath"
}

# Main execution
Write-Header "Caesar Performance Benchmark Suite"

if (!(Test-Prerequisites)) {
    Write-Error "Prerequisites not met. Please install missing components."
    exit 1
}

# Build C++ benchmarks
Build-CppBenchmarks

# Run benchmarks
if ($Category -eq "all") {
    foreach ($cat in $Categories.GetEnumerator()) {
        Invoke-CategoryBenchmark -CategoryName $cat.Key -CategoryInfo $cat.Value
    }
} elseif ($Categories.ContainsKey($Category)) {
    Invoke-CategoryBenchmark -CategoryName $Category -CategoryInfo $Categories[$Category]
} else {
    Write-Error "Unknown category: $Category"
    Write-Host "Available categories: $($Categories.Keys -join ', ')"
    exit 1
}

# Generate reports
Write-SummaryReport

if ($OutputCsv) {
    Export-ResultsToCsv
}

Write-Host "`n" -NoNewline
Write-Success "Benchmark suite completed!"
Write-Host "Total tests run: $($Results.Count)"
Write-Host "Iterations per test: $Iterations"