{
  "targets": [
    {
      "target_name": "pv",
      "sources": [ 
        "src/binding.c"
        # "src/async.h",
        # "src/addon.c",
        # "src/async.cc" 
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "include_dirs": [
 #       "<!@(node -p \"require('node-addon-api').include\")",
        "/usr/include/samba-4.0",
        "/usr/include/node/"
      ],
      "libraries": [
        "/usr/lib/x86_64-linux-gnu/libsmbclient.so"
      ],
      # "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
#      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1 },
      }
    }
  ]
}
