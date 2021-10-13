var stats = require('../tools/stats.js');
var request = require('../tools/smb2-forge').request;
var SMB2Forge = require('../tools/smb2-forge');
var SMB2Request = SMB2Forge.request;

module.exports = function stat(path, cb) {
  var connection = this;

  // function queryInfo(filesBatch, file, connection, cb) {
  //   SMB2Request('query_directory', file, connection, function(err, files) {
  //     if (err) {
  //       if (err.code === 'STATUS_NO_MORE_FILES') {
  //         cb(null, filesBatch);
  //       } else {
  //         cb(err);
  //       }
  //     } else {
  //       filesBatch.push(
  //         files
  //           .filter(function(v) {
  //             // remove '.' and '..' values
  //             return v.Filename !== '.' && v.Filename !== '..';
  //           })
  //           .map(mapping)
  //       );
  //       queryDirectory(filesBatch, file, connection, cb);
  //     }
  //   });
  // }



  request('open', { path: path }, connection, function(err, file) {
    if (err != null) {
      return cb(err);
    }
    request('close', file, connection, function() {
      cb(null, stats(file));
    });
  });
};
