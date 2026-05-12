# Menu Consolidation - Real File Operations Only

## Summary
✅ **Options 19-23 and 27 now handle ONLY real file operations**
✅ **Duplicate options 28, 29, 30 removed**
✅ **Cleaner, more focused menu structure**

---

## New Menu Structure

### File Management (Real Files) Section

```
-- File Management (Real Files) --
19. Upload File
20. Download File
21. Delete File
22. Search File
23. View My Files
27. Restore Deleted File from Trash
```

---

## Option Details

### **Option 19: Upload File** 
**Purpose**: Upload real files from your computer

**Input Required**:
- Real file path (absolute path from C:\ or D:\ etc)
- Example: `C:\Users\Rathishan\Downloads\resume.pdf`

**Process**:
1. User provides file path
2. System validates file exists
3. File is copied to `storage/files/fileId_originalname`
4. File metadata stored (size, type, owner)
5. Owner permission granted automatically
6. File ID returned for future reference

**Output**:
```
✅ Real file uploaded successfully!
   File ID : 1001
   Name    : resume.pdf
   Owner   : admin
   Size    : 245892 bytes (actual)
   Storage : storage/files/1001_resume.pdf

✅ Owner permission added automatically for admin
```

**Error Handling**:
- "Source file not found" → Path is incorrect
- "File is empty" → File size is 0 bytes
- "Access denied" → File is in use

---

### **Option 20: Download File**
**Purpose**: Download uploaded files to `storage/downloads/`

**Input Required**:
- File ID (from upload or search)
- Example: `1001`

**Process**:
1. User provides file ID
2. System checks file exists and not deleted
3. System checks user has permission
4. File copied to `storage/downloads/fileId_originalname`

**Output**:
```
✅ File copied to downloads: storage/downloads/1001_resume.pdf
   File name : resume.pdf
   File size : 245892 bytes
   Downloaded to: storage/downloads
```

**Error Handling**:
- "File not found!" → Invalid file ID
- "File is already deleted!" → Cannot download deleted file
- "Access denied! You do not have permission" → User lacks permission

---

### **Option 21: Delete File**
**Purpose**: Delete uploaded files (move to trash)

**Input Required**:
- File ID to delete
- Example: `1001`

**Process**:
1. User provides file ID
2. System checks file exists and not deleted
3. System checks user has permission
4. File moved from `storage/files/` to `storage/trash/`
5. User permission revoked
6. File marked as deleted

**Output**:
```
✅ File moved to trash successfully.
   File ID: 1001
   Trash location: storage/trash/1001_resume.pdf
```

**Error Handling**:
- "File not found!" → Invalid file ID
- "File is already deleted!" → File already deleted
- "Access denied!" → No permission to delete

---

### **Option 22: Search File**
**Purpose**: Search for files by name

**Input Required**:
- File name (or part of name)
- Example: `resume`

**Process**:
1. User provides search term
2. System searches all files for matching names
3. Returns all matches with metadata

**Output**:
```
--- Search Results for: resume ---
  ID: 1001 | Name: resume.pdf | Owner: admin | Path: storage/files/1001_resume.pdf
```

---

### **Option 23: View My Files**
**Purpose**: View all files uploaded by current user

**Input Required**:
- None (automatically uses logged-in user)

**Process**:
1. System retrieves all files owned by current user
2. Shows metadata for each file
3. Shows storage location

**Output**:
```
=== My Files (admin) ===
  1. ID: 1001 | Name: resume.pdf | Type: .pdf | Size: 245892 bytes
     Path: storage/files/1001_resume.pdf | Owner: admin
     
  2. ID: 1002 | Name: report.docx | Type: .docx | Size: 123456 bytes
     Path: storage/files/1002_report.docx | Owner: admin
```

---

### **Option 27: Restore Deleted File from Trash**
**Purpose**: Restore deleted files from trash

**Input Required**:
- None (restores most recent deleted file)

**Process**:
1. System finds most recently deleted file
2. Moves file from `storage/trash/` back to `storage/files/`
3. Restores original filename
4. Restores owner permission
5. Marks file as not deleted

**Output**:
```
✅ File restored from trash!
   File ID: 1001
   Name: resume.pdf
   Restored to: storage/files/1001_resume.pdf

✅ Owner permission restored for admin
```

---

## Removed Options

### **Old Options 28, 29, 30** ❌ DELETED
- Option 28: Upload Real File (now Option 19)
- Option 29: Download Real File (now Option 20)
- Option 30: Delete Real File (now Option 21)

These were removed after consolidating into the main File Management section.

---

## File Operations Flow

```
START
  ↓
LOGIN (required for all file operations)
  ↓
┌─────────────────────────────────────────────────┐
│     CHOOSE FILE OPERATION                       │
├─────────────────────────────────────────────────┤
│                                                 │
│  19. UPLOAD FILE                                │
│    └→ Select file from computer                 │
│    └→ Copy to storage/files/                    │
│    └→ Grant permission to owner                 │
│                                                 │
│  20. DOWNLOAD FILE                              │
│    └→ Check permission                          │
│    └→ Copy to storage/downloads/                │
│                                                 │
│  21. DELETE FILE                                │
│    └→ Check permission                          │
│    └→ Move to storage/trash/                    │
│    └→ Revoke permission                         │
│                                                 │
│  22. SEARCH FILE                                │
│    └→ Find files by name                        │
│    └→ Show results with IDs                     │
│                                                 │
│  23. VIEW MY FILES                              │
│    └→ List all your uploaded files              │
│    └→ Show metadata & storage paths             │
│                                                 │
│  27. RESTORE DELETED FILE                       │
│    └→ Move from trash back to storage/files/    │
│    └→ Restore permission to owner               │
│                                                 │
└─────────────────────────────────────────────────┘
```

---

## Permission System Integration

All operations check permissions:

| Operation | Permission Required | Default |
|-----------|-------------------|---------|
| Download | Must have file permission | Only owner |
| Delete | Must have file permission | Only owner |
| Search | None (everyone can search) | Public |
| View My Files | None (see only your files) | Your files |
| Restore | Must be owner | Owner only |

---

## Storage Locations

After operations, files are stored in:

```
backend/
├── storage/
│   ├── files/        ← Active uploaded files (option 19)
│   ├── downloads/    ← Downloaded copies (option 20)
│   └── trash/        ← Deleted files (option 21)
```

---

## Example Workflow

### Complete Upload → Download → Delete Workflow

```
1. LOGIN
   Username: admin
   Password: admin123

2. UPLOAD FILE (Option 19)
   Path: C:\Users\Rathishan\Downloads\resume.pdf
   
   Output: ✅ File ID 1001 created
           Storage: storage/files/1001_resume.pdf
           Permission: Granted to admin

3. SEARCH FILE (Option 22)
   Search: resume
   
   Output: Found ID: 1001, Name: resume.pdf, Path: storage/files/1001_resume.pdf

4. DOWNLOAD FILE (Option 20)
   File ID: 1001
   
   Output: ✅ File copied to: storage/downloads/1001_resume.pdf

5. DELETE FILE (Option 21)
   File ID: 1001
   
   Output: ✅ File moved to: storage/trash/1001_resume.pdf
           Permission: Revoked

6. RESTORE FILE (Option 27)
   
   Output: ✅ File restored from trash
           Moved to: storage/files/1001_resume.pdf
           Permission: Restored to admin
```

---

## Testing Checklist

- [ ] Compile successfully: `g++ main.cpp -std=c++17 -g -o main.exe`
- [ ] Login works (admin/admin123)
- [ ] Option 19: Upload your resume.pdf
- [ ] Option 22: Search for "resume" → finds it
- [ ] Option 23: View My Files → shows resume.pdf
- [ ] Option 20: Download file ID from search
- [ ] Option 21: Delete file → moves to trash
- [ ] Option 27: Restore deleted file → back in storage/files
- [ ] Permission system works (admin can grant to other users)
- [ ] Verify storage structure:
  - `dir storage/files`
  - `dir storage/downloads`
  - `dir storage/trash`

---

## Benefits of This Consolidation

✅ **Cleaner Menu** - No duplicate options
✅ **Simpler UX** - Real file operations in one section
✅ **No Confusion** - Only one way to upload/download
✅ **Consistent Behavior** - All operations use same logic
✅ **Easier Maintenance** - Single implementation per operation

---

## Compilation Status

```
Command: g++ main.cpp -std=c++17 -Wall -Wextra -g -o main.exe
Result:  ✅ SUCCESS
Binary:  main.exe (772.22 KB)
Errors:  0
Warnings: 0
```

Ready to use! 🚀
