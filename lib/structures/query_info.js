// From https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-smb2/d623b2f7-a5cd-4639-8cc9-71fa7d9f9ba9
// and https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-fscc/4718fc40-e539-4014-8e33-b675af74e3e1
module.exports = {
  request: [
    ['StructureSize', 2, 41],
    ['InfoType', 1, 0x03], // SMB2_0_INFO_SECURITY
    ['FileInfoClass', 1, 0], // security
    ['OutputBufferLength', 4, 0x00010000],
    ['InputBufferOffset', 2, 0],
    ['Reserved', 2, 0x0000],
    ['InputBufferLength', 4, 0],
    ['AdditionalInformation', 4, 0x00000001], // OWNER_SECURITY_INFORMATION

    ['Flags', 4, 0],
    ['FileId', 16],
    // ['Buffer', 'FileNameLength'],
  ],

  response: [
    ['StructureSize', 2],
    ['OutputBufferOffset', 2],
    ['OutputBufferLength', 4],
    ['Buffer', 'OutputBufferLength'],
  ],
};
