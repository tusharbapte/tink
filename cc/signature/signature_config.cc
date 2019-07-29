// Copyright 2017 Google Inc.
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

#include "tink/signature/signature_config.h"

#include "absl/memory/memory.h"
#include "tink/config/config_util.h"
#include "tink/registry.h"
#include "tink/signature/ecdsa_sign_key_manager.h"
#include "tink/signature/ed25519_sign_key_manager.h"
#include "tink/signature/rsa_ssa_pkcs1_sign_key_manager.h"
#include "tink/signature/rsa_ssa_pss_sign_key_manager.h"
#include "tink/signature/ecdsa_verify_key_manager.h"
#include "tink/signature/ed25519_verify_key_manager.h"
#include "tink/signature/rsa_ssa_pkcs1_verify_key_manager.h"
#include "tink/signature/rsa_ssa_pss_verify_key_manager.h"
#include "tink/signature/public_key_sign_wrapper.h"
#include "tink/signature/public_key_verify_wrapper.h"
#include "tink/util/status.h"
#include "proto/config.pb.h"

namespace crypto {
namespace tink {

namespace {

google::crypto::tink::RegistryConfig* GenerateRegistryConfig() {
  google::crypto::tink::RegistryConfig* config =
      new google::crypto::tink::RegistryConfig();
  // ECDSA
  config->add_entry()->MergeFrom(
      CreateTinkKeyTypeEntry(SignatureConfig::kPublicKeySignCatalogueName,
                                   SignatureConfig::kPublicKeySignPrimitiveName,
                                   "EcdsaPrivateKey", 0, true));
  config->add_entry()->MergeFrom(CreateTinkKeyTypeEntry(
      SignatureConfig::kPublicKeyVerifyCatalogueName,
      SignatureConfig::kPublicKeyVerifyPrimitiveName, "EcdsaPublicKey", 0,
      true));

  // ED25519
  config->add_entry()->MergeFrom(
      CreateTinkKeyTypeEntry(SignatureConfig::kPublicKeySignCatalogueName,
                                   SignatureConfig::kPublicKeySignPrimitiveName,
                                   "Ed25519PrivateKey", 0, true));
  config->add_entry()->MergeFrom(CreateTinkKeyTypeEntry(
      SignatureConfig::kPublicKeyVerifyCatalogueName,
      SignatureConfig::kPublicKeyVerifyPrimitiveName, "Ed25519PublicKey", 0,
      true));

  // RSA SSA PSS
  config->add_entry()->MergeFrom(
      CreateTinkKeyTypeEntry(SignatureConfig::kPublicKeySignCatalogueName,
                                   SignatureConfig::kPublicKeySignPrimitiveName,
                                   "RsaSsaPssPrivateKey", 0, true));
  config->add_entry()->MergeFrom(CreateTinkKeyTypeEntry(
      SignatureConfig::kPublicKeyVerifyCatalogueName,
      SignatureConfig::kPublicKeyVerifyPrimitiveName, "RsaSsaPssPublicKey", 0,
      true));

  // RSA SSA PKCS1
  config->add_entry()->MergeFrom(
      CreateTinkKeyTypeEntry(SignatureConfig::kPublicKeySignCatalogueName,
                                   SignatureConfig::kPublicKeySignPrimitiveName,
                                   "RsaSsaPkcs1PrivateKey", 0, true));
  config->add_entry()->MergeFrom(CreateTinkKeyTypeEntry(
      SignatureConfig::kPublicKeyVerifyCatalogueName,
      SignatureConfig::kPublicKeyVerifyPrimitiveName, "RsaSsaPkcs1PublicKey", 0,
      true));
  config->set_config_name("TINK_SIGNATURE");
  return config;
}

}  // anonymous namespace

constexpr char SignatureConfig::kPublicKeySignCatalogueName[];
constexpr char SignatureConfig::kPublicKeyVerifyCatalogueName[];
constexpr char SignatureConfig::kPublicKeySignPrimitiveName[];
constexpr char SignatureConfig::kPublicKeyVerifyPrimitiveName[];

// static
const google::crypto::tink::RegistryConfig& SignatureConfig::Latest() {
  static const auto config = GenerateRegistryConfig();
  return *config;
}

// static
util::Status SignatureConfig::Register() {
  // ECDSA
  auto status = Registry::RegisterKeyManager(
      absl::make_unique<EcdsaSignKeyManager>(), true);
  if (!status.ok()) return status;
  status = Registry::RegisterKeyManager(
      absl::make_unique<EcdsaVerifyKeyManager>(), true);
  if (!status.ok()) return status;

  // ED25519
  status = Registry::RegisterKeyManager(
      absl::make_unique<Ed25519SignKeyManager>(), true);
  if (!status.ok()) return status;
  status = Registry::RegisterKeyManager(
      absl::make_unique<Ed25519VerifyKeyManager>(), true);
  if (!status.ok()) return status;

  // RSA SSA PSS
  status = Registry::RegisterKeyManager(
      absl::make_unique<RsaSsaPssSignKeyManager>(), true);
  if (!status.ok()) return status;
  status = Registry::RegisterKeyManager(
      absl::make_unique<RsaSsaPssVerifyKeyManager>(), true);
  if (!status.ok()) return status;

  // RSA SSA PKCS1
  status = Registry::RegisterKeyManager(
      absl::make_unique<RsaSsaPkcs1SignKeyManager>(), true);
  if (!status.ok()) return status;
  status = Registry::RegisterKeyManager(
      absl::make_unique<RsaSsaPkcs1VerifyKeyManager>(), true);
  if (!status.ok()) return status;

  status = Registry::RegisterPrimitiveWrapper(
      absl::make_unique<PublicKeySignWrapper>());
  if (!status.ok()) return status;
  return  Registry::RegisterPrimitiveWrapper(
      absl::make_unique<PublicKeyVerifyWrapper>());
}

}  // namespace tink
}  // namespace crypto
