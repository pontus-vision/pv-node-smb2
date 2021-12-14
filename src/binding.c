#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <libsmbclient.h>
#include <node_api.h>

#define PV_MAX(a, b) ((a) > (b) ? (a) : (b))
#define PV_MIN(a, b) ((a) < (b) ? (a) : (b))

const ssize_t MAX_FILE_SIZE = (50 * 1024 * 1024);

#define PV_MAX_CRED_SIZE 2048
typedef struct PV_SMB2_CREDS_STRUCT
{
  char workgroup[PV_MAX_CRED_SIZE];
  char username[PV_MAX_CRED_SIZE];
  char password[PV_MAX_CRED_SIZE];
} PV_SMB2_CREDS_STRUCT, *PV_SMB2_CREDS;

void pvAuthFunc(SMBCCTX *context,
                const char *srv,
                const char *shr,
                char *wg, int wglen,
                char *un, int unlen,
                char *pw, int pwlen)
{
  PV_SMB2_CREDS creds = (PV_SMB2_CREDS)smbc_getOptionUserData(context);

  smbc_set_credentials_with_fallback(context, creds->workgroup, creds->username, creds->password);

  strncpy(wg, creds->workgroup, PV_MIN(wglen, PV_MAX_CRED_SIZE));
  strncpy(un, creds->username, PV_MIN(unlen, PV_MAX_CRED_SIZE));
  strncpy(pw, creds->password, PV_MIN(pwlen, PV_MAX_CRED_SIZE));
}

// Limit ourselves to this many primes, starting at 2
// #define PRIME_COUNT 10
#define CHECK(expr)                                                                 \
  {                                                                                 \
    if ((expr) == 0)                                                                \
    {                                                                               \
      fprintf(stderr, "%s:%d: failed assertion `%s'\n", __FILE__, __LINE__, #expr); \
      fflush(stderr);                                                               \
      abort();                                                                      \
    }                                                                               \
  }

static void pvDeleteContextBase(SMBCCTX *ctx)
{
  if (ctx)
  {
    PV_SMB2_CREDS creds = smbc_getOptionUserData(ctx);
    if (creds)
    {
      free(creds);
    }
    smbc_free_context(ctx, 1);
  }
}

static void pvDeleteContextFinaliser(napi_env env,
                                     void *finalize_data,
                                     void *finalize_hint)
{

  SMBCCTX *ctx = (SMBCCTX *)finalize_data;
  pvDeleteContextBase(ctx);
}

static napi_value pvDeleteContext(napi_env env, napi_callback_info info)
{
  size_t argc = 1;
  napi_value args[1];
  CHECK(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  if (argc < 1)
  {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }

  napi_valuetype valuetype0;
  CHECK(napi_typeof(env, args[0], &valuetype0) == napi_ok);

  if (valuetype0 != napi_external)
  {
    napi_throw_type_error(env, NULL, "Wrong argument type");
    return NULL;
  }

  SMBCCTX *ctx;

  CHECK(napi_get_value_external(env, args[0], (void **)&ctx) == napi_ok);

  pvDeleteContextBase(ctx);

  napi_value isOk, isOkBool;
  CHECK(napi_create_uint32(env, 1, &isOk) == napi_ok);
  CHECK(napi_coerce_to_bool(env, isOk, &isOkBool) == napi_ok);

  return isOkBool;
}

static napi_value pvCreateContext(napi_env env, napi_callback_info info)
{
  // char *workgroup = "MD2";
  // char *user = "leonardo.martins";
  // char *password = "Asdf#1234qwer";

  size_t argc = 3;
  napi_value args[3];
  CHECK(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  if (argc < 3)
  {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }

  napi_valuetype valuetype0;
  CHECK(napi_typeof(env, args[0], &valuetype0) == napi_ok);

  napi_valuetype valuetype1;
  CHECK(napi_typeof(env, args[1], &valuetype1) == napi_ok);

  napi_valuetype valuetype2;
  CHECK(napi_typeof(env, args[2], &valuetype2) == napi_ok);

  if (valuetype0 != napi_string || valuetype1 != napi_string || valuetype2 != napi_string)
  {
    napi_throw_type_error(env, NULL, "Wrong argument types");
    return NULL;
  }

  SMBCCTX *context = smbc_new_context();

  context = smbc_init_context(context);

  smbc_setFunctionAuthDataWithContext(context, (smbc_get_auth_data_with_context_fn)pvAuthFunc);

  PV_SMB2_CREDS creds = calloc(1, sizeof(PV_SMB2_CREDS_STRUCT));

  smbc_setOptionUserData(context, creds);

  CHECK(napi_get_value_string_utf8(env, args[0], creds->workgroup, PV_MAX_CRED_SIZE, NULL) == napi_ok);
  CHECK(napi_get_value_string_utf8(env, args[1], creds->username, PV_MAX_CRED_SIZE, NULL) == napi_ok);
  CHECK(napi_get_value_string_utf8(env, args[2], creds->password, PV_MAX_CRED_SIZE, NULL) == napi_ok);

  napi_value ctx;
  CHECK(napi_create_external(env, context, pvDeleteContextFinaliser, NULL, &ctx) == napi_ok);

  return ctx;
}

static napi_value pvDirReadFromContext(napi_env env, napi_callback_info info)
{
  // char *workgroup = "MD2";
  // char *user = "leonardo.martins";
  // char *password = "Asdf#1234qwer";

  size_t argc = 2;
  napi_value args[2];
  CHECK(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  if (argc < 2)
  {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }

  napi_valuetype valuetype0;
  CHECK(napi_typeof(env, args[0], &valuetype0) == napi_ok);

  napi_valuetype valuetype1;
  CHECK(napi_typeof(env, args[1], &valuetype1) == napi_ok);

  if (valuetype0 != napi_external || valuetype1 != napi_string)
  {
    napi_throw_type_error(env, NULL, "Wrong argument types");
    return NULL;
  }

  SMBCCTX *ctx;

  CHECK(napi_get_value_external(env, args[0], (void **)&ctx) == napi_ok);
  char url[PV_MAX_CRED_SIZE];
  CHECK(napi_get_value_string_utf8(env, args[1], url, PV_MAX_CRED_SIZE, NULL) == napi_ok);

  napi_value retVal;

  CHECK(napi_create_array(env, &retVal) == napi_ok);

  int counter = 0;
  SMBCFILE *fd = (smbc_getFunctionOpendir(ctx))(ctx, url);
  if (!fd)
  {
    char msg[PV_MAX_CRED_SIZE];
    snprintf(msg, PV_MAX_CRED_SIZE, "Unable to open directory; error %s", strerror(errno));
    napi_throw_type_error(env, NULL, msg);
    return NULL;
  }

  const struct libsmb_file_info *fileInfo = NULL;

  do
  {
    errno = 0;
    fileInfo = (smbc_getFunctionReaddirPlus(ctx))(ctx, fd);

    if (fileInfo == NULL && errno != 0)
    {
      smbc_getFunctionClose(ctx)(ctx, fd);
      char msg[PV_MAX_CRED_SIZE];
      snprintf(msg, PV_MAX_CRED_SIZE, "Unable to read directory; error %s", strerror(errno));
      napi_throw_type_error(env, NULL, msg);

      return NULL;
    }
    else if (fileInfo == NULL)
    {
      break;
    }

    napi_value obj;
    CHECK(napi_create_object(env, &obj) == napi_ok);

    napi_value size;
    CHECK(napi_create_uint32(env, fileInfo->size, &size) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "size", size) == napi_ok);

    napi_value uid;
    CHECK(napi_create_uint32(env, fileInfo->uid, &uid) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "uid", uid) == napi_ok);

    napi_value gid;
    CHECK(napi_create_uint32(env, fileInfo->gid, &gid) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "gid", gid) == napi_ok);

    napi_value btime_ts;
    CHECK(napi_create_uint32(env, fileInfo->btime_ts.tv_sec, &btime_ts) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "btime_ts", btime_ts) == napi_ok);

    napi_value mtime_ts;
    CHECK(napi_create_uint32(env, fileInfo->mtime_ts.tv_sec, &mtime_ts) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "mtime_ts", mtime_ts) == napi_ok);

    napi_value atime_ts;
    CHECK(napi_create_uint32(env, fileInfo->atime_ts.tv_sec, &atime_ts) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "uid", uid) == napi_ok);

    napi_value ctime_ts;
    CHECK(napi_create_uint32(env, fileInfo->ctime_ts.tv_sec, &ctime_ts) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "ctime_ts", ctime_ts) == napi_ok);

    napi_value name;

    char *nameStr = fileInfo->name ? fileInfo->name : fileInfo->short_name;
    printf("Got  %s [%d]\n", nameStr, counter);

    if (nameStr)
    {
      printf("Got %s \n", nameStr);
      CHECK(napi_create_string_utf8(env, nameStr, strlen(nameStr), &name) == napi_ok);
      CHECK(napi_set_named_property(env, obj, "name", name) == napi_ok);
    }

    napi_value isFile, isFileBool;
    CHECK(napi_create_uint32(env, fileInfo->attrs & SMBC_DOS_MODE_ARCHIVE, &isFile) == napi_ok);
    CHECK(napi_coerce_to_bool(env, isFile, &isFileBool) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "isFile", isFileBool) == napi_ok);

    napi_value isDir, isDirBool;
    CHECK(napi_create_uint32(env, fileInfo->attrs & SMBC_DOS_MODE_DIRECTORY, &isDir) == napi_ok);
    CHECK(napi_coerce_to_bool(env, isDir, &isDirBool) == napi_ok);
    CHECK(napi_set_named_property(env, obj, "isDir", isDirBool) == napi_ok);

    CHECK(napi_set_element(env, retVal, counter, obj) == napi_ok);
    counter++;

  } while (info != NULL);

  smbc_getFunctionClose(ctx)(ctx, fd);

  return retVal;
}

static napi_value pvReadFromContext(napi_env env, napi_callback_info info)
{
  // char *workgroup = "MD2";
  // char *user = "leonardo.martins";
  // char *password = "Asdf#1234qwer";

  size_t argc = 2;
  napi_value args[2];
  CHECK(napi_get_cb_info(env, info, &argc, args, NULL, NULL) == napi_ok);

  if (argc < 2)
  {
    napi_throw_type_error(env, NULL, "Wrong number of arguments");
    return NULL;
  }

  napi_valuetype valuetype0;
  CHECK(napi_typeof(env, args[0], &valuetype0) == napi_ok);

  napi_valuetype valuetype1;
  CHECK(napi_typeof(env, args[1], &valuetype1) == napi_ok);

  if (valuetype0 != napi_external || valuetype1 != napi_string)
  {
    napi_throw_type_error(env, NULL, "Wrong argument types");
    return NULL;
  }

  SMBCCTX *ctx;

  CHECK(napi_get_value_external(env, args[0], (void **)&ctx) == napi_ok);
  char url[PV_MAX_CRED_SIZE];
  CHECK(napi_get_value_string_utf8(env, args[1], url, PV_MAX_CRED_SIZE, NULL) == napi_ok);

  char *buf = calloc(1, MAX_FILE_SIZE);
  if (!buf)
  {
    char msg[PV_MAX_CRED_SIZE];
    snprintf(msg, PV_MAX_CRED_SIZE, "Unable allocate %ld bytes; error %s", MAX_FILE_SIZE, strerror(errno));
    napi_throw_type_error(env, NULL, msg);
    return NULL;
  }

  SMBCFILE *fd = (smbc_getFunctionOpen(ctx))(ctx, url, O_RDONLY, 0444);

  if (!fd)
  {
    char msg[PV_MAX_CRED_SIZE];
    snprintf(msg, PV_MAX_CRED_SIZE, "Unable to open file; error %s", strerror(errno));
    napi_throw_type_error(env, NULL, msg);
    return NULL;
  }

  ssize_t numBytes = (smbc_getFunctionRead(ctx))(ctx, fd, buf, MAX_FILE_SIZE);

  smbc_getFunctionClose(ctx)(ctx, fd);

  napi_value retVal;
  CHECK(napi_create_buffer_copy(env, numBytes, buf, NULL, &retVal) == napi_ok);

  free(buf);

  return retVal;
}

// The commented-out return type and the commented out formal function
// parameters below help us keep in mind the signature of the addon
// initialization function. We write the body as though the return value were as
// commented below and as though there were parameters passed in as commented
// below.
/*napi_value*/ NAPI_MODULE_INIT(/*napi_env env, napi_value exports*/)
{

  napi_property_descriptor pvReadFromContextFunc = {
      "pvReadFromContext",
      NULL,
      pvReadFromContext,
      NULL,
      NULL,
      NULL,
      napi_default,
      NULL};

  // Decorate exports with the above-defined properties.
  CHECK(napi_define_properties(env, exports, 1, &pvReadFromContextFunc) == napi_ok);

  napi_property_descriptor pvCreateContextFunc = {
      "pvCreateContext",
      NULL,
      pvCreateContext,
      NULL,
      NULL,
      NULL,
      napi_default,
      NULL};

  // Decorate exports with the above-defined properties.
  CHECK(napi_define_properties(env, exports, 1, &pvCreateContextFunc) == napi_ok);

  napi_property_descriptor pvDeleteContextFunc = {
      "pvDeleteContext",
      NULL,
      pvDeleteContext,
      NULL,
      NULL,
      NULL,
      napi_default,
      NULL};

  // Decorate exports with the above-defined properties.
  CHECK(napi_define_properties(env, exports, 1, &pvDeleteContextFunc) == napi_ok);

  napi_property_descriptor pvDirReadFromContextFunc = {
      "pvDirReadFromContext",
      NULL,
      pvDirReadFromContext,
      NULL,
      NULL,
      NULL,
      napi_default,
      NULL};

  // Decorate exports with the above-defined properties.
  CHECK(napi_define_properties(env, exports, 1, &pvDirReadFromContextFunc) == napi_ok);

  // Return the decorated exports object.
  return exports;
}