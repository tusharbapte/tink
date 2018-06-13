// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////

goog.module('tink.KeyManager');

const PbKeyData = goog.require('proto.google.crypto.tink.KeyData');
const PbMessage = goog.require('jspb.Message');
const SecurityException = goog.require('tink.exception.SecurityException');

/**
 * An auxiliary container for methods that generate new keys.
 * Those methods are separate from KeyManager as their functionality is
 * independent of the primitive of the corresponding KeyManager.
 *
 * @record
 */
class KeyFactory {
  /**
   * Generates a new random key according to 'keyFormat'.
   *
   * @param {!PbMessage|string} keyFormat is either a KeyFormat proto or a
   *     serialized KeyFormat proto
   * @return {!Promise.<!PbMessage>} the new generated key
   * @throws {SecurityException} if the specified format is not supported
   */
  newKey(keyFormat) {}

  /**
   * Generates a new random key based on the "serialized_key_format" and returns
   * it as a KeyData proto.
   *
   * @param {string} serializedKeyFormat
   * @return {!Promise.<!PbKeyData>}
   * @throws {SecurityException} if the specified format is not supported
   */
  newKeyData(serializedKeyFormat) {}
}

/**
 * A KeyManager "understands" keys of a specific key type: it can generate keys
 * of the supported type and create primitives for supported keys.
 * A key type is identified by the global name of the protocol buffer that holds
 * the corresponding key material, and is given by typeUrl-field of
 * KeyData-protocol buffer.
 *
 * The template parameter P denotes the primitive corresponding to the keys
 * handled by this manager.
 *
 * @template P
 * @record
 */
class KeyManager {
  /**
   * Constructs an instance of primitive P for a given key.
   *
   * @param {!PbKeyData|!PbMessage} key is either a KeyData proto or a supported
   *     key proto
   * @return {!Promise.<!P>}
   * @throws {SecurityExeception} if the given key is corrupted or not
   *     supported
   */
  getPrimitive(key) {}

  /**
   * Returns true if this KeyManager supports keyType.
   *
   * @param {string} keyType
   * @return {boolean}
   */
  doesSupport(keyType) {}

  /**
   * Returns the URL which identifies the keys managed by this KeyManager.
   *
   * @return {string}
   */
  getKeyType() {}

  /**
   * Returns the version of this KeyManager.
   *
   * @return {number}
   */
  getVersion() {}

  /**
   * Returns a factory that generates keys of the key type handled by this
   * manager.
   *
   * @return {KeyFactory}
   */
  getKeyFactory() {}
}

exports = {
  KeyManager,
  KeyFactory
};
