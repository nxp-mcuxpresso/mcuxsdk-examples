.. highlight:: bat

.. _ex-sss-md:

=======================================================================
 Message Digest Example
=======================================================================

This project demonstrates a Message Digest / hashing operation using SSS APIs.

Prerequisites
=======================================================================

- Build Plug and Trust middleware stack. (Refer :ref:`building`)


About the Example
=======================================================================
This example does a Message Digest hashing operation.

It uses the following APIs and data types:
  - :cpp:func:`sss_digest_context_init()`
  - :cpp:enumerator:`kAlgorithm_SSS_SHA1` from :cpp:type:`sss_algorithm_t`
  - :cpp:enumerator:`kMode_SSS_Digest` from :cpp:type:`sss_mode_t`
  - :cpp:func:`sss_digest_one_go()`


Console output
=======================================================================


#) If everything is setup correctly the output would be as follows ::

      App:INFO :Do Digest
      App:INFO :input (Len=10)
          48 65 6C 6C   6F 57 6F 72   6C 64
      App:INFO :Message Digest successful !!!
      App:INFO :digest (Len=20)
          DB 8A C1 C2   59 EB 89 D4   A1 31 B2 53   BA CF CA 5F
          31 9D 54 F2
      App:INFO :ex_sss_digest Example Success !!!...
      Press any key to continue . . .


