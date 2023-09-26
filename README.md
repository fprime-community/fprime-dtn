# F´ DTN Integration

Delay-Tolerant Networking in F´.

With support from the 9x Interplanetary Network Directorate this development aims to accomplish the following objectives:
- Integrate ION into F´.
- Develop modular DTN components for use in F´ projects.
- Integrate ION into the F´ GDS.
- Demonstrate DTN in a multi-node environment using F´ and F´ GDS end-to-end.

For detailed documentation see [`docs/`](docs).

## Getting Started

### Dependencies

- [Docker](https://www.docker.com/)

### Git Submodules

To fully clone all Git submodules within the `lib/` directory run:
```
git submodule update --init --recursive
```

## Maintenance

### Git Submodules

To update all submodules to their latest version run:
```
git submodule update --remote
```

## Resources

- [DTN Overview](https://www.nasa.gov/sites/default/files/atoms/files/dtn_tutorial_v3.2_0.pdf)
- [F´](https://github.com/nasa/fprime)
- [F´ Course](https://github.com/fprime-community/fprime-system-reference/blob/main/docs/course/introduction.md)
- [FPP User's Guide](https://fprime-community.github.io/fpp/fpp-users-guide.html)
- [ION](https://www.nasa.gov/directorates/heo/scan/engineering/technology/disruption_tolerant_networking_software_options_ion)
- [ION-Core](https://github.com/nasa-jpl/ion-core.git)
- [`pyion`](https://pyion.readthedocs.io/en/latest/index.html)
