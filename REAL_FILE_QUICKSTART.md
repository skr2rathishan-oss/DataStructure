# Quick Start Guide - Real File Support

## Compile
```bash
cd backend
g++ main.cpp -std=c++17 -g -o main.exe
```

## Run
```bash
.\main.exe
```

## Menu Options (NEW)
```
28. Upload Real File      - Upload a file from your computer
29. Download Real File    - Download stored file to storage/downloads/
30. Delete Real File      - Delete file (move to trash)
```

## Step-by-Step Examples

### Example 1: Upload a Real File
```
1. Choose: 1 (Login)
   Username: admin
   Password: admin123

2. Choose: 28 (Upload Real File)
   Enter path: C:\Users\YourName\Documents\report.pdf
   
Output:
   ✅ File copied to storage: storage/files/1001_report.pdf
   ✅ Real file uploaded successfully!
   File ID: 1001
   Owner: admin
   Type: .pdf
   Size: 245892 bytes (actual)
```

### Example 2: Download Your File
```
1. Login (as admin or user with permission)

2. Choose: 29 (Download Real File)
   Enter file ID: 1001
   
Output:
   ✅ File copied to downloads: storage/downloads/1001_report.pdf
   File name: report.pdf
   File size: 245892 bytes
   Downloaded to: storage/downloads
```

### Example 3: Delete a File
```
1. Login (as file owner or admin)

2. Choose: 30 (Delete Real File)
   Enter file ID: 1001
   
Output:
   ✅ File moved to trash: storage/trash/1001_report.pdf
   File deleted and moved to trash.
```

### Example 4: Share with Another User
```
1. Login as admin

2. Upload file (option 28)

3. Grant permission (option 4)
   Username: user1
   File name: report.pdf
   
Output:
   Permission granted successfully.

4. (Other user) Login as user1

5. (Other user) Download (option 29)
   File ID: 1001
   
Output:
   ✅ Download successful!
```

## Storage Locations

After running the program:
```
backend/
├── storage/
│   ├── files/          ← Uploaded files stored here
│   │   └── 1001_report.pdf
│   ├── downloads/      ← Downloaded files copied here
│   │   └── 1001_report.pdf
│   └── trash/          ← Deleted files moved here
│       └── 1001_report.pdf
```

## File Naming Convention
- **Format**: `fileId_originalFileName`
- **Example**: `1001_report.pdf`
- **Why**: Prevents name collisions, keeps original name with ID

## Key Features

✅ **Real File Operations**
- Copy actual files from computer
- Store in organized folder structure
- Download to local folder
- Move to trash on delete

✅ **Permission Integration**
- Owner gets automatic permission on upload
- Admin can grant/revoke permissions
- Users can only access files they have permission for
- Permission revoked on delete

✅ **Existing Features Still Work**
- Metadata-only uploads (option 19)
- File search (option 22)
- View my files (option 23)
- Permission management (options 4-7)
- Undo (option 27)

## Error Handling

**If you see these messages:**

| Message | Cause | Solution |
|---------|-------|----------|
| "Source file not found" | Path doesn't exist | Check file path is correct, use absolute path |
| "File is empty" | File size is 0 bytes | Upload a file with content |
| "Access denied" | No permission | Ask admin to grant permission |
| "File not found" | File ID doesn't exist | Check file ID is correct (use option 22 to search) |
| "This file is metadata-only" | Uploaded via option 19 | Use option 19 for metadata files, option 29 for real files |

## Testing

### Create a Test File
```bash
cd backend
"This is a test file" | Out-File test_files\sample.txt
```

### Test Upload
```
Choose: 28
Path: [full path to test_files\sample.txt]
```

### Test Download
```
Choose: 29
File ID: [ID from upload, usually 1001]
```

### Verify Storage
```bash
# Show all files
dir storage /s

# or
# Check files folder
dir storage/files
# Check downloads folder
dir storage/downloads
# Check trash folder
dir storage/trash
```

## Notes

- File paths must be absolute (full path from root)
- Example: `C:\Users\Rathishan\Documents\file.pdf`
- NOT: `Documents\file.pdf` or `~/file.pdf`
- Case-sensitive on Linux, case-insensitive on Windows
- Files stored with ID prefix: `1001_`, `1002_`, etc.
- Original filename and extension preserved

## Limitations

- Local storage only (no cloud yet)
- Single-threaded (one user at a time in console)
- No file size limits enforced
- No encryption
- In-memory session (data lost after exit)

## Next Steps (For Deployment)

1. Add persistent database storage
2. Connect to cloud storage (S3, Azure)
3. Add multi-user concurrent access
4. Add file encryption
5. Add HTTP API for remote access
6. Add web frontend
