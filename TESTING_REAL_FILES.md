# How to Test Real File Support

## Prerequisites
```bash
cd backend
```

## Compilation
```bash
g++ main.cpp -std=c++17 -Wall -Wextra -g -o main.exe
```

## Test Scenario 1: Upload and Download in One Session

```bash
.\main.exe
```

Then in the application:
```
1. Choose 1 (Login)
   Username: admin
   Password: admin123

2. Choose 28 (Upload Real File)
   Enter path: [full absolute path to any file on your computer]
   Example: C:\Users\Rathishan\Desktop\Git\Data_Structure\SmartCloudStorage\backend\test_files\sample_document.txt

3. Choose 22 (Search File)
   Enter name: sample_document.txt
   See: ID: 1001 | Name: sample_document.txt | Path: storage/files/1001_sample_document.txt

4. Choose 29 (Download Real File)
   Enter file ID: 1001
   See: File copied to downloads: storage/downloads/1001_sample_document.txt

5. Choose 0 (Exit)
```

After this, check:
```bash
dir storage/files        # Shows: 1001_sample_document.txt
dir storage/downloads    # Shows: 1001_sample_document.txt
```

## Test Scenario 2: Delete File to Trash

```bash
.\main.exe
```

Then:
```
1. Login (admin/admin123)

2. Upload file (28)
   Path: [your test file path]

3. Delete real file (30)
   Enter file ID: 1001

4. Exit
```

After this, check:
```bash
dir storage/files        # Empty (file was deleted)
dir storage/trash        # Shows: 1001_[filename]
```

## Test Scenario 3: Permission System with Real Files

Terminal 1 - Admin uploads and grants permission:
```bash
.\main.exe
```
```
1. Login admin/admin123
2. Upload file (28) → ID becomes 1001
3. Grant permission (4)
   Username: user1
   File: [filename]
4. Exit
```

Terminal 2 - User downloads:
```bash
.\main.exe
```
```
1. Login user1/user123
2. Download (29) → File ID: 1001
3. Success!
4. Exit
```

Check result:
```bash
dir storage/downloads    # Shows: 1001_[filename]
```

## Test Scenario 4: User Without Permission

```bash
.\main.exe
```
```
1. Login user1/user123
2. Try download (29) → File ID: 1001
3. See: "Access denied! You do not have permission"
```

## Verify Storage Structure

```bash
# Show entire storage
tree storage /F
# or
dir storage /s /b

# Expected output:
# storage\files
# storage\downloads
# storage\trash
```

## Create Test Files

If you want to create test files for upload:

```powershell
# Create test directory
mkdir test_files -Force

# Create test file 1
"This is a test document for real file upload testing" | Out-File test_files\document.txt

# Create test file 2
"Important data: 123, 456, 789" | Out-File test_files\data.txt

# Create a slightly larger file
$content = "Line " + ((1..100) -join "`nLine "); 
$content | Out-File test_files\large_file.txt
```

Then use full path when uploading:
```
C:\Users\Rathishan\Desktop\Git\Data_Structure\SmartCloudStorage\backend\test_files\document.txt
```

## Example Upload Session

```
Login as admin (admin/admin123)

Option 28 - Upload Real File
Enter path: C:\Users\Rathishan\Desktop\Git\Data_Structure\SmartCloudStorage\backend\test_files\document.txt

Output:
✅ File copied to storage: storage/files/1001_document.txt
✅ Real file uploaded successfully!
   File ID : 1001
   Name    : document.txt
   Owner   : admin
   Type    : .txt
   Size    : 54 bytes (actual)
   Storage : storage/files/1001_document.txt
✅ Owner permission added automatically for admin

Option 22 - Search File
Enter name: document.txt
Output:
--- Search Results for: document.txt ---
  ID: 1001 | Name: document.txt | Owner: admin | Path: storage/files/1001_document.txt

Option 29 - Download Real File
Enter file ID: 1001
Output:
✅ File copied to downloads: storage/downloads/1001_document.txt
   File name : document.txt
   File size : 54 bytes
   Downloaded to: storage/downloads

Option 30 - Delete Real File
Enter file ID: 1001
Output:
✅ File moved to trash: storage/trash/1001_document.txt
   File moved to trash successfully.
   File deleted and moved to trash.
```

## File Organization After Testing

```
backend/
├── storage/
│   ├── files/                    # Empty after delete
│   ├── downloads/
│   │   └── 1001_document.txt    # Downloaded copy
│   └── trash/
│       └── 1001_document.txt    # Deleted file
├── test_files/
│   ├── document.txt
│   ├── data.txt
│   └── large_file.txt
├── main.exe                      # Compiled binary
└── main.cpp                      # Source code
```

## Troubleshooting

**Problem**: "Source file not found"
**Solution**: 
- Use absolute path (from C:\ or D:\ etc)
- Check path is typed correctly
- File must exist and be readable

**Problem**: "File is empty"
**Solution**:
- Create test file with content first
- Don't upload 0-byte files

**Problem**: "Access denied"
**Solution**:
- Login as correct user
- Admin must grant permission first
- Check permission with option 5

**Problem**: File not showing in storage/
**Solution**:
- Upload must succeed (check for error messages)
- Verify with file explorer: `dir storage/files`
- Try absolute path like: `C:\Users\[name]\test\file.txt`

## Next Steps After Testing

1. ✅ Test with your own files from any folder
2. ✅ Test permission system thoroughly
3. ✅ Test delete and trash functionality
4. ✅ Verify search finds uploaded real files
5. ✅ Check file sizes match original files
6. ✅ Clean up storage/ folder between tests

## Clean Storage Between Tests

```bash
# Remove storage folder
rmdir /s /q storage 2>nul

# or in PowerShell
Remove-Item -Path storage -Recurse -Force 2>$null

# Storage will be recreated automatically when app runs
```

## Performance Notes

- Copying large files may take time depending on disk speed
- First upload takes longer (creates storage directories)
- Small files (< 10MB) copy instantly
- Local filesystem operations are fast (no network)

## Compatibility

- Windows: ✅ Yes (tested)
- Linux/Mac: ✅ Yes (C++17 filesystem works cross-platform)
- Requires: C++17 compiler (g++ 7+, clang 5+)

## Next Deployment Steps

1. Add database to persist uploaded files
2. Add user authentication with hashed passwords
3. Connect to cloud storage (AWS S3, Azure Blob)
4. Add HTTP API for remote access
5. Add web frontend
6. Add file encryption at rest
