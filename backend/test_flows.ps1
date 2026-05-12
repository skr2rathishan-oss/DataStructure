# Test script for SmartCloudStorage backend
# Tests all critical flows

$exe = ".\main.exe"

# Test inputs as comma-separated strings
# Format: menu_choice, param1, param2, ...

# Test 1: Invalid login
Write-Host "`n=== TEST 1: Invalid Login ===" -ForegroundColor Cyan
$input1 = @"
1
invaliduser
wrongpass
0
"@
$input1 | & $exe

# Test 2: Admin login
Write-Host "`n=== TEST 2: Admin Login ===" -ForegroundColor Cyan
$input2 = @"
1
admin
admin123
2
3
0
"@
$input2 | & $exe

# Test 3-4: Upload file and search
Write-Host "`n=== TEST 3: Upload File ===" -ForegroundColor Cyan
$input3 = @"
1
admin
admin123
19
report.pdf
/uploads/report.pdf
PDF
2048
22
report.pdf
23
0
"@
$input3 | & $exe

# Test 5: Download file
Write-Host "`n=== TEST 5: Download File ===" -ForegroundColor Cyan
$input5 = @"
1
admin
admin123
20
1001
0
"@
$input5 | & $exe

# Test 6: Delete file
Write-Host "`n=== TEST 6: Delete File ===" -ForegroundColor Cyan
$input6 = @"
1
admin
admin123
21
1001
0
"@
$input6 | & $exe

# Test 7: Restore file
Write-Host "`n=== TEST 7: Restore File ===" -ForegroundColor Cyan
$input7 = @"
1
admin
admin123
27
20
1001
0
"@
$input7 | & $exe

# Test 8: Download restored file
Write-Host "`n=== TEST 8: Download Restored File ===" -ForegroundColor Cyan
$input8 = @"
1
admin
admin123
20
1001
0
"@
$input8 | & $exe

# Test 9: Grant permission to user
Write-Host "`n=== TEST 9: Grant Permission ===" -ForegroundColor Cyan
$input9 = @"
1
admin
admin123
4
user1
report.pdf
0
"@
$input9 | & $exe

# Test 10: User login and download permitted file
Write-Host "`n=== TEST 10: User Login and Download ===" -ForegroundColor Cyan
$input10 = @"
1
user1
user123
20
1001
0
"@
$input10 | & $exe

# Test 11: User cannot process request
Write-Host "`n=== TEST 11: User Cannot Process Request ===" -ForegroundColor Cyan
$input11 = @"
1
user1
user123
10
0
"@
$input11 | & $exe

# Test 12: Admin processes request
Write-Host "`n=== TEST 12: Admin Process Request ===" -ForegroundColor Cyan
$input12 = @"
1
admin
admin123
10
0
"@
$input12 | & $exe

Write-Host "`n=== ALL TESTS COMPLETED ===" -ForegroundColor Green
