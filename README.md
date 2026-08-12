# Clinical Non-Contact Infrared Thermometer
> **Klinik Temassız Kızılötesi Termometre Projesi**  
> *BME425 Biyomedikal Mühendisliği Projesi | Group 1 (01-MIY-2526S)*

---

## 📌 Project Overview

This repository contains the design, hardware specifications, PCB Gerber files, mechanical enclosure models, testing procedures, and clinical standard compliance documentation for a **Clinical-Grade Non-Contact Infrared (IR) Thermometer**.

The device is engineered according to international medical device standards, specifically **EN ISO 80601-2-56:2017** (*Particular requirements for basic safety and essential performance of clinical thermometers for body temperature measurement*).

---

## 🔬 Key Specifications & Hardware Architecture

| Component | Description / Part Number | Interface |
| :--- | :--- | :--- |
| **Microcontroller Unit (MCU)** | ESP32-C3 Mini WiFi & Bluetooth LE Board | I2C / GPIO |
| **Infrared Temperature Sensor** | Melexis GY-906 / MLX90614 Non-Contact IR Sensor | I2C |
| **Display Unit** | 0.96-inch SSD1306 OLED Graphic Display (128x64) | I2C |
| **Power Management** | TP4056 Type-C Lithium Battery Charger Module | Power Input |
| **Battery** | Power-Xtra PX302030 3.7V 120mAh 1S Li-Polymer Battery | 3.7V DC |
| **Audio Notification** | 3–24V Active Electronic Buzzer | GPIO Alert Output |
| **Enclosure** | Custom Ergonomic 3D Printable Housing | Mechanical |

---

## 📁 Repository Navigation Structure

```
Clinical-NoContact-IR-Thermometer/
├── README.md                                    # Main project documentation & specification guide
├── .gitignore                                   # OS & editor file exclusion rules
│
├── docs/                                        # Documentation & Regulatory Files
│   ├── reports/                                 # Project plans, design descriptions & final reports
│   │   ├── BME425_Thermometer_Document.pdf      # Detailed thermometer design overview
│   │   ├── Final_Report_Group1.pdf              # Final capstone project report
│   │   ├── Project_Management_Plan.docx         # Project Management Plan (PMP)
│   │   ├── Design_Description.docx              # System Design Description
│   │   ├── System_Requirements.docx             # System requirements specification
│   │   ├── Preliminary_Design_Review.docx       # Preliminary Design Review (PDR)
│   │   ├── Configuration_Item_List.docx         # Configuration management item list
│   │   ├── Group1_Gantt_Chart.xlsx              # Project timeline & Gantt chart
│   │   └── Appendix.docx                        # Documentation appendix
│   │
│   ├── test-procedures/                         # Verification, Validation & Acceptance Testing
│   │   ├── Acceptance_Test_Procedure_ATP.pdf    # Acceptance Test Procedure (ATP)
│   │   ├── Verification_Validation_Test_Procedure_V2TP.pdf # V2TP test procedure
│   │   ├── Verification_Validation_Test_Plan.pdf# Verification & validation test plan
│   │   ├── Power_And_Control_Board_Assembly.pdf # Assembly guide for power/control board
│   │   ├── Assembly_Record.pdf                  # Device assembly records
│   │   ├── Verification_Validation_Test_Procedure_Template.docx # V2TP template
│   │   └── V2TP_Test_Datasheet.xlsx             # Test data recording spreadsheet
│   │
│   ├── standards/                               # Regulatory Medical Standards
│   │   ├── EN_ISO_80601-2-56-2017.pdf           # ISO standard for clinical thermometers (PDF)
│   │   └── EN_ISO_80601-2-56-2017.docx          # ISO standard reference document (Word)
│   │
│   ├── sustainability/                          # Sustainability & Ethics Declarations
│   │   ├── UN_Sustainable_Development_Report.pdf# UN SDG alignment report
│   │   ├── UN_Sustainable_Development_Affirmation.docx # Sustainability affirmation
│   │   ├── Sustainable.docx                     # Sustainability impact analysis
│   │   └── Ethics_Declaration.docx              # Engineering ethics declaration
│   │
│   └── reference-examples/                      # Past sample files & template references
│
├── hardware/                                    # Hardware & Mechanical Engineering
│   ├── datasheets/                              # Component Datasheets
│   │   ├── ESP32-C3_Mini_Board.pdf              # ESP32-C3 MCU datasheet
│   │   ├── MLX90614_IR_Sensor.pdf               # MLX90614 IR temperature sensor datasheet
│   │   ├── SSD1306_OLED_Display.pdf             # SSD1306 0.96" OLED display datasheet
│   │   ├── TP4056_Battery_Charger.pdf           # TP4056 Li-Po charger datasheet
│   │   ├── Power-Xtra_PX302030_LiPo_Battery.pdf # 3.7V 120mAh LiPo battery datasheet
│   │   └── Active_Buzzer.pdf                    # Active electronic buzzer datasheet
│   │
│   ├── pcb/                                     # Gerber PCB Fabrication Files
│   │   ├── Design-F_Cu.gbr                      # Top copper layer
│   │   ├── Design-B_Cu.gbr                      # Bottom copper layer
│   │   ├── Design-F_Mask.gbr                    # Top solder mask
│   │   ├── Design-B_Mask.gbr                    # Bottom solder mask
│   │   ├── Design-F_Silkscreen.gbr              # Top silkscreen
│   │   ├── Design-B_Silkscreen.gbr              # Bottom silkscreen
│   │   ├── Design-F_Paste.gbr                   # Top solder paste
│   │   ├── Design-B_Paste.gbr                   # Bottom solder paste
│   │   └── Design-Edge_Cuts.gbr                 # Board outline / edge cuts
│   │
│   └── mechanical/                              # Housing Design & Enclosure Specifications
│       ├── Housing_Design_v10.docx              # Enclosure design document
│       ├── Housing_Design_v10.pdf               # Housing design specifications PDF
│       ├── Housing_Assembly.pdf                 # Enclosure assembly instructions
│       ├── Cover_Page.docx                      # Housing cover documentation
│       └── Cover_Page.pdf                       # Housing cover documentation PDF
│
└── archive/                                     # Archival Storage
    └── duplicates/                              # Preserved previous draft versions & duplicates
```

---

## 📐 Medical Standards Compliance

This project complies with **EN ISO 80601-2-56:2017**, which governs:
- Clinical accuracy and repeatability in non-contact body temperature measurements.
- Operational safety, thermal limit protections, and power management.
- Verification & Validation test procedure protocols (V2TP / ATP).

---

## 🧪 Verification & Acceptance Testing

Testing procedures are structured under `docs/test-procedures/`:
1. **Acceptance Test Procedure (ATP)**: Validates basic physical and electrical assembly requirements.
2. **Verification & Validation Test Procedure (V2TP)**: Verifies sensor calibration accuracy, thermal response time, audio alerts, and display output against reference laboratory thermometers.
3. **Assembly Records**: Documents component soldering, board integration, and housing assembly steps.

---

## 📄 License & Attribution

Developed for **BME425 (01-MIY-2526S) Biomedical Engineering Project**. All rights reserved by Group 1 contributors.
