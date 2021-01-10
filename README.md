# Port-scanner project [Started 31/12/2020]
---

:pushpin: The Project was implemented in C++.

:pushpin: This is multi-threaded port-scanner.

---
### How to build & Usage

    mkdir build && cd build/ && cmake .. && make -j && ./scanner {input.json}

    Example of the contents of an input file :
    "input.json" :
    {
        "Targets": [
            {
                "Host": "google.com",
                "PortRange": "1-1050"
            }
        ],
        "Options": {
            "Verbose": true,
            "Timeout": 3
        }
    } 
---

