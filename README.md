<!--
  Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
  See https://llvm.org/LICENSE.txt for license information.
  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

> [!IMPORTANT]
>
> QInfo is experimental at this stage, and significant changes, including breaking changes, are
> anticipated in upcoming releases.

# QInfo

The `QInfo` library is part of the _Munich Quantum Software Stack (MQSS)_, which is a project of the
_Munich Quantum Valley (MQV)_ initiative and is jointly developed by the _Leibniz Supercomputing
Centre (LRZ)_ and the Chairs for _Design Automation (CDA)_, and for _Computer Architecture and
Parallel Systems (CAPS)_ at TUM.

The `QInfo` library provides a simple and flexible way to pass additional information to the library
routines of the MQSS, such as hints for the library routines or additional information about the
quantum device. It takes inspiration from the `MPI_Info` object in the Message Passing Interface
(MPI) standard. Effectively, the `QInfo` object is an opaque object that stores an unordered set of
key-value pairs, where the keys are strings and the values can be integers, floats, or strings.

## Contact

The development of this project is led by [Laura Schulz](mailto:laura.schulz@lrz.de) (LRZ),
[Martin Schulz](mailto:martin.w.j.schulz@tum.de) (TUM CAPS), and
[Robert Wille](mailto:robert.wille@tum.de) (TUM CDA) on the management side and
[Lukas Burgholzer](mailto:lukas.burgholzer@tum.de) (TUM CDA) as well as
[Jorge Echavarria](mailto:jorge.echavarria@lrz.de) (LRZ) from the technical side.

Please try to use the publicly accessible GitHub channels
([issues](https://github.com/Munich-Quantum-Software-Stack/QInfo/issues),
[discussions](https://github.com/Munich-Quantum-Software-Stack/QInfo/discussions),
[pull requests](https://github.com/Munich-Quantum-Software-Stack/QInfo/pulls)) to allow for a
transparent and open discussion as much as possible.

## License

The `QInfo` library is released under the Apache License v2.0 with LLVM Exceptions. See
[LICENSE](LICENSE) for more information. Any contribution to the project is assumed to be under the
same license.
