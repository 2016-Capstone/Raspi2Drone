/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
/*
 * GENERATED FILE
 *  Do not modify this file, it will be erased during the next configure run 
 */

package com.parrot.arsdk.arsal;

import java.util.HashMap;

/**
 * Java copy of the eARSAL_SOCKET_CLASS_SELECTOR enum
 */
public enum ARSAL_SOCKET_CLASS_SELECTOR_ENUM {
   /** Dummy value for all unknown cases */
    eARSAL_SOCKET_CLASS_SELECTOR_UNKNOWN_ENUM_VALUE (Integer.MIN_VALUE, "Dummy value for all unknown cases"),
   ARSAL_SOCKET_CLASS_SELECTOR_CS0 (0),
   ARSAL_SOCKET_CLASS_SELECTOR_UNSPECIFIED (0),
   ARSAL_SOCKET_CLASS_SELECTOR_CS1 (32),
   ARSAL_SOCKET_CLASS_SELECTOR_CS2 (64),
   ARSAL_SOCKET_CLASS_SELECTOR_CS3 (96),
   ARSAL_SOCKET_CLASS_SELECTOR_CS4 (128),
   ARSAL_SOCKET_CLASS_SELECTOR_CS5 (160),
   ARSAL_SOCKET_CLASS_SELECTOR_CS6 (192),
   ARSAL_SOCKET_CLASS_SELECTOR_CS7 (224);

    private final int value;
    private final String comment;
    static HashMap<Integer, ARSAL_SOCKET_CLASS_SELECTOR_ENUM> valuesList;

    ARSAL_SOCKET_CLASS_SELECTOR_ENUM (int value) {
        this.value = value;
        this.comment = null;
    }

    ARSAL_SOCKET_CLASS_SELECTOR_ENUM (int value, String comment) {
        this.value = value;
        this.comment = comment;
    }

    /**
     * Gets the int value of the enum
     * @return int value of the enum
     */
    public int getValue () {
        return value;
    }

    /**
     * Gets the ARSAL_SOCKET_CLASS_SELECTOR_ENUM instance from a C enum value
     * @param value C value of the enum
     * @return The ARSAL_SOCKET_CLASS_SELECTOR_ENUM instance, or null if the C enum value was not valid
     */
    public static ARSAL_SOCKET_CLASS_SELECTOR_ENUM getFromValue (int value) {
        if (null == valuesList) {
            ARSAL_SOCKET_CLASS_SELECTOR_ENUM [] valuesArray = ARSAL_SOCKET_CLASS_SELECTOR_ENUM.values ();
            valuesList = new HashMap<Integer, ARSAL_SOCKET_CLASS_SELECTOR_ENUM> (valuesArray.length);
            for (ARSAL_SOCKET_CLASS_SELECTOR_ENUM entry : valuesArray) {
                valuesList.put (entry.getValue (), entry);
            }
        }
        ARSAL_SOCKET_CLASS_SELECTOR_ENUM retVal = valuesList.get (value);
        if (retVal == null) {
            retVal = eARSAL_SOCKET_CLASS_SELECTOR_UNKNOWN_ENUM_VALUE;
        }
        return retVal;    }

    /**
     * Returns the enum comment as a description string
     * @return The enum description
     */
    public String toString () {
        if (this.comment != null) {
            return this.comment;
        }
        return super.toString ();
    }
}
