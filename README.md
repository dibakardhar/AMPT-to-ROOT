# AMPT to ROOT Data Converter

## 📌 Overview
This project is a ROOT-based data processing script that converts raw output from the **AMPT (A Multi-Phase Transport Model)** simulation into structured **ROOT files**. These ROOT files can then be used for further analysis in high-energy nuclear and particle physics studies.

---

## ⚛️ Physics Context
This tool is designed for processing output from AMPT (A Multi-Phase Transport Model),
commonly used in relativistic heavy-ion collision studies to analyze particle production,
flow, and transport properties.

---

## ⚙️ Features
- Reads raw AMPT-generated event data
- Extracts relevant physics quantities
- Converts data into ROOT TTrees
- Produces `.root` files for analysis
- Lightweight and easy to integrate into analysis workflows

---

## 🚀 Requirements
Make sure you have the following installed:

- [ROOT Framework](https://root.cern)
- C++ compatible environment (usually included with ROOT)

---

## ▶️ How to Run

Open ROOT and execute the script:

```bash
root -l ampt_to_root.C
```

---

## 📖 Citation

If you use this code in your research or publication, please cite it as:
Dibakar Dhar, "AMPT to ROOT Data Converter", GitHub repository, 2026. https://github.com/dibakardhar/AMPT-to-ROOT
