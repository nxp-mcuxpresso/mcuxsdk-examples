# The BSD 3 Clause License
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN if ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

mcux_set_variable(NXP_OT_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/ot-nxp")
mcux_set_variable(NXP_OT_LIBS_PATH "${NXP_OT_ROOT_PATH}/build_rw612/rw612_ot_cli/lib")

if (EXISTS ${NXP_OT_ROOT_PATH})
    mcux_add_include(
        BASE_PATH ${NXP_OT_ROOT_PATH}
        INCLUDES
            # rw612 header files
            src/rw/rw612

            # ot common
            src/common

            # openthread
            openthread/src
            openthread/include
            openthread/src/core
            openthread/examples/platforms
            openthread/third_party/mbedtls

            # mbedtls config in third_party
            third_party/mbedtls/configs
    )
else()
    message(WARNING "Please download ot-nxp in ${CMAKE_CURRENT_LIST_DIR}")
endif()

if (EXISTS ${NXP_OT_LIBS_PATH})
    # ot ncp libs
    mcux_add_library(
        BASE_PATH ${NXP_OT_LIBS_PATH}
        LIBS
            libopenthread-cli-ftd.a
            libopenthread-ftd.a
            libopenthread-hdlc.a
            libopenthread-ncp-ftd.a
            libopenthread-platform.a
            libopenthread-rw612.a
            libopenthread-spinel-ncp.a
            libot-cli-addons.a
            libot-cli-rw612.a
            libopenthread-url.a
            libopenthread-radio-spinel.a
            libtcplp-ftd.a
    )
else()
    message(WARNING "Please compile ot ncp libs in ${CMAKE_CURRENT_LIST_DIR}")
endif()
