# SMB2 Client for Node.js

[![Node compatibility](https://badgen.net/npm/node/pv-node-smb2)](https://npmjs.org/package/pv-node-smb2) [![License](https://badgen.net/npm/license/pv-node-smb2)](https://npmjs.org/package/pv-node-smb2) [![PackagePhobia](https://badgen.net/packagephobia/install/pv-node-smb2)](https://packagephobia.now.sh/result?p=pv-node-smb2)

[![Package Version](https://badgen.net/npm/v/pv-node-smb2)](https://npmjs.org/package/pv-node-smb2) [![Latest Commit](https://badgen.net/github/last-commit/Node-SMB/marsaud-smb2)](https://github.com/Node-SMB/marsaud-smb2/commits/master)

## Introduction

This library calls the C libraries for libsmbclient to read and search through directories.

The original fork was unable to connect to newer servers, and rather than hacking about with the smb2 protocol, it is far easier to 
leverage the great work of the samba team, and use their libsmbclient library via NAPI bindings.
This is much quicker than other wrappers that fork/exec smbclient, and the only OS pre-req (on Linux) is to install the libsmbclient-dev package.
## Installation

```bash
npm install -S pv-node-smb2
```

## API

### Asynchronicity

No async calls supported yet; there's a skeleton for a read function, but much more work is still needed.


### Sync calls
```typescript

import {PvDirent,PvNodeSmb2} from 'pv-node-smb2'

const smb2Client = new PvNodeSmb2('workgroup', 'user', 'password')

const content = await smb2Client.readFileSync('smb://<host or IP>/Windows Path/goes/here/foo.txt')
console.log(content.toString('base64')
```


### Connection management

Each instance of the PvNodeSmb2 class will have its own connection.


## Contributors

- [Leo Martins](https://github.com/pontusvision)

## References

    The[MS-SMB2]: Server Message Block (SMB) Protocol Versions 2 and 3
    Copyright (C) 2014 Microsoft
    http://msdn.microsoft.com/en-us/library/cc246482.aspx

## License

(The MIT License)

Copyright (c) 2013-2014 Benjamin Chelli &lt;benjamin@chelli.net&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
