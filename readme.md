# C INI Parser
---

## Goal of Project
The main objective of this project is to demonstrate (and for me to learn) how to work with INI configuration files in C and share these configurations through a REST API using the Ulfius library. This provides a practical example of interfacing with configuration files programmatically and how it's possible to make them accessible over a network.

## Modes (progress 0/3)

### Write (default) (TODO)
This mode is able to read ini file and save whole file, or only some sections of it in cleaner form to standard output or file.
I plan for it to be similar to `jq` tool for json.

### Modify (cli) (TODO)
This mode allowes you to make some changes to configuration. You can add/delete/rename sections,  add/delete/modify specific key-value records, or group/split content of sections.

### Publish (api) (TODO)
This mode is able to publish vales to be read from ~~d-bus or~~ local http by some other programs. 

## ROADMAP

### prototypes:

 - 01-prototype: version that is able to build and not much more
 - 02-prototype: version with working parser and config
 - 03-prototype: version with working stdin input and organized output
 - 03-prototype: version with working stdin input and organized output

### alpha:

- alpha-01: version with working parsing and `Write` jq-like functions
- alpha-02: version with working parsing and `Modify` console
- alpha-03: version with working parsing and `Publish` api intergarion

### beta:

- beta-01: Integration of `Write`, `Modify` and `Publish` modes
- beta-02: Control of API from `Modify` console

### Release:

- c-ini-parser-0.1: first stable release



## Future:

maybe this project will be transformed into c-toml-parser? Who knows?
