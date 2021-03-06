# Randomness Testing Toolkit
Randomness testing toolkit (RTT) is a tool that unites execution of various statistical batteries. Its aim is to detect non-randomness in given binary data in simple and automated manner. RTT also post-processes outputs of batteries so one does not need to cope vith various output formats these batteries use. Advanced users can even implement their own output module with minimal complications.

RTT currently implements support for following statistical tools:
* **NIST Statistical Test Suite** [home page](http://csrc.nist.gov/groups/ST/toolkit/rng/documentation_software.html)
* **Dieharder: A Random Number Test Suite** [home page](http://www.phy.duke.edu/~rgb/General/dieharder.php)
* **TestU01** [home page](http://simul.iro.umontreal.ca/testu01/tu01.html)

## Related repositories
This repository contains backend part of Randomness Testing Toolkit. For other parts see following repositories.

* **Statistical batteries** [GitHub](https://github.com/crocs-muni/rtt-statistical-batteries)  
Set of updated and modified statistical batteries compatible with RTT. 
* **Service deployment automation** [GitHub](https://github.com/crocs-muni/rtt-deployment)  
RTT can be deployed on single or multiple servers as a service. Scripts in this repository automates the process of deployment.
* **Web interface** [GitHub](https://github.com/crocs-muni/RTTWebInterface)  
The web interface is working on top of the deployed service and allow users to analyze data and view the analysis results through their browsers.

## Installation and usage
For instructions on how to install and use Randomness Testing Toolkit, please refer to our [wiki](https://github.com/crocs-muni/randomness-testing-toolkit/wiki).

## Authors
The framework is developed at the [Centre for Research on Cryptography and Security (formerly Laboratory of Security and Applied Cryptography)](https://www.fi.muni.cz/research/crocs/), [Masaryk University](http://www.muni.cz/), Brno, Czech Republic.

* **Petr Švenda** 2015-now (Project lead)
* **Ľubomír Obrátil** 2015-now (Development)

## License
Randomness Testing Toolkit is licenced under MIT Licence, Copyright (c) 2015-2017 Centre for Research on Cryptography and Security. For details on MIT Licence, see file LICENSE.

## Third party libraries
Some sub-parts of the project have their own licencing conditions. The brief list of such sub-parts follows.

* **Easylogging++**  [Home page](https://github.com/muflihun/easyloggingpp)  
Simple header C++ logging library. Released under MIT license.

* **JSON for modern C++** [Home page](https://github.com/nlohmann/json)  
JSON parser for C++. Released under MIT license.

* **C++17 std::variant for C++11/14/17** [Home page](https://github.com/mpark/variant)  
Implementation of variant object from C++17 standard.

