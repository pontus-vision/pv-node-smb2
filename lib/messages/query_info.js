var SMB2Message = require('../tools/smb2-message');
var message = require('../tools/message');

module.exports = message({
  generate: function(connection, params) {
    return new SMB2Message({
      headers: {
        Command: 'QUERY_INFO',
        SessionId: connection.SessionId,
        TreeId: connection.TreeId,
        ProcessId: connection.ProcessId,
      },
      request: {
        FileId: params.FileId
      },
    });
  },

  parseResponse: function(response) {
    return parseFiles(response.getResponse().Buffer);
  },
});

/*
 * HELPERS
 */
function parseFiles(buffer) {
  let files = [];
  let nextFileOffset = -1;
  // from https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-fscc/95f3056a-ebc1-4f5d-b938-3f68a44677a6
  let offset = 4;
  let len = 4;


  const OffsetOwner = buffer.readUInt32LE (offset);

  buffer.readUInt32LE


  while (nextFileOffset !== 0) {
    // extract next file offset
    nextFileOffset = buffer.readUInt32LE(offset);
    // extract the file
    files.push(
      parseFile(
        buffer.slice(
          offset + 4,
          nextFileOffset ? offset + nextFileOffset : buffer.length
        )
      )
    );
    // move to nex file
    offset += nextFileOffset;
  }
  return files;
}

function parseFile(buffer) {
  var file = {};
  var offset = 0;

  // index
  file.Index = buffer.readUInt32LE(offset);
  offset += 4;

  // CreationTime
  file.CreationTime = buffer.slice(offset, offset + 8);
  offset += 8;

  // LastAccessTime
  file.LastAccessTime = buffer.slice(offset, offset + 8);
  offset += 8;

  // LastWriteTime
  file.LastWriteTime = buffer.slice(offset, offset + 8);
  offset += 8;

  // ChangeTime
  file.ChangeTime = buffer.slice(offset, offset + 8);
  offset += 8;

  // EndofFile
  file.EndofFile = buffer.slice(offset, offset + 8);
  offset += 8;

  // AllocationSize
  file.AllocationSize = buffer.slice(offset, offset + 8);
  offset += 8;

  // FileAttributes
  file.FileAttributes = buffer.readUInt32LE(offset);
  offset += 4;

  // FilenameLength
  file.FilenameLength = buffer.readUInt32LE(offset);
  offset += 4;

  // EASize
  file.EASize = buffer.readUInt32LE(offset);
  offset += 4;

  // ShortNameLength
  file.ShortNameLength = buffer.readUInt8(offset);
  offset += 1;

  // FileId
  file.FileId = buffer.slice(offset, offset + 8);
  offset += 8;

  // Reserved
  offset += 27;

  // Filename
  file.Filename = buffer
    .slice(offset, offset + file.FilenameLength)
    .toString('ucs2');
  offset += file.FilenameLength;

  return file;
}
