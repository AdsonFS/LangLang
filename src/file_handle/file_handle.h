#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

#include <string>
class FileHandle {
public:
  static std::string fileName;
  static std::string fileContent;
  static int fileSize;

  static std::string getLineError(int position) {
    int start = position, end = position;
    while (start > 0 && FileHandle::fileContent[start - 1] != '\n')
      --start;
    if (FileHandle::fileContent[end] == '\n' && end > 0)
      end--;
    while (FileHandle::fileContent[end + 1] != '\0' &&
           FileHandle::fileContent[end + 1] != '\n')
      ++end;
    return FileHandle::fileContent.substr(start, end - start + 1);
  };
};

#endif // FILE_HANDLE_H
