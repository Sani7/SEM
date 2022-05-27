#pragma once

bool readSetting(char *fileContent, int contentLength, const char *settingName, char *returnBuff, int returnLength);
bool readFileContents(const char *fileName, char *content, int length);