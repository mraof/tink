// Copyright 2019 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////

#include "tink/python/cc/cc_streaming_aead_wrappers.h"

#include "third_party/pybind11/include/pybind11/pybind11.h"
#include "third_party/pybind11_abseil/absl_casters.h"
#include "tink/python/cc/clif/import_helper.h"
#include "tink/python/cc/clif/status_casters.h"

namespace crypto {
namespace tink {

PYBIND11_MODULE(cc_streaming_aead_wrappers, m) {
  namespace py = pybind11;

  ImportTinkPythonModule("python.cc.clif.input_stream_adapter");
  ImportTinkPythonModule("python.cc.clif.output_stream_adapter");
  ImportTinkPythonModule("python.cc.clif.python_file_object_adapter");

  // TODO(b/146492561): Reduce the number of complicated lambdas.
  m.def(
      "new_cc_encrypting_stream",
      // TODO(b/145925674)
      [](StreamingAead* streaming_aead, const py::bytes& aad,
         std::shared_ptr<PythonFileObjectAdapter> ciphertext_destination)
          -> util::StatusOr<std::unique_ptr<OutputStreamAdapter>> {
        return NewCcEncryptingStream(streaming_aead, std::string(aad),
                                     ciphertext_destination);
      },
      py::arg("primitive"), py::arg("aad"), py::arg("destination"),
      // Keep destination alive at least as long as OutputStreamAdapter.
      py::keep_alive<0, 3>());

  m.def(
      "new_cc_decrypting_stream",
      // TODO(b/145925674)
      [](StreamingAead* streaming_aead, const py::bytes& aad,
         std::shared_ptr<PythonFileObjectAdapter> ciphertext_source)
          -> util::StatusOr<std::unique_ptr<InputStreamAdapter>> {
        return NewCcDecryptingStream(streaming_aead, std::string(aad),
                                     ciphertext_source);
      },
      py::arg("primitive"), py::arg("aad"), py::arg("source"),
      // Keep source alive at least as long as InputStreamAdapter.
      py::keep_alive<0, 3>());
}

}  // namespace tink
}  // namespace crypto
