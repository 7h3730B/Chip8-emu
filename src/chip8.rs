use crate::constants;
use crate::cpu::Cpu;

use std::fs::File;
use std::io::prelude::*;

pub struct Chip8 {
    pub cpu: Cpu,
    pub memory: [u8; constants::RAM_SIZE],
    pub stack: [u16; constants::STACK_SIZE],
    pub keys: [u8; constants::N_KEYS],
}

impl Chip8 {
    pub fn new() -> Self {
        let mut memory = [0; constants::RAM_SIZE];
        let mut stack = [0; constants::STACK_SIZE];
        let mut keys = [0; constants::N_KEYS];
        
        let mut cpu = Cpu {
            pc: constants::PROGRAM_START,
            I: 0,
            sp: 0,
            registers: [0; constants::N_REGISTERS]
        };

        Self {
            cpu,
            memory,
            stack,
            keys
        }
    }
    
    pub fn setup_from_file(self: &mut Self, filename: &str) {
        let rom = self.read_from_file(&filename);
        self.read_into_memory(&rom);
        self.read_fonts_into_memory();
    }
    
    fn read_from_file(self: &Self, filename: &str) -> &[u8] {
        println!("Reading file: {}", &filename);
        let mut file = File::open(filename).unwrap();
        let mut rom = Vec::new();
        file.read_to_end(&mut rom).unwrap();
        &rom
    }
    
    fn read_into_memory(self: &mut Self, rom: &[u8]) {
        println!("writing rom into memory");
        let rom_size = rom.len();
        self.memory[constants::PROGRAM_START..rom_size].copy_from_slice(&rom);
    }
    
    fn read_fonts_into_memory(self: &mut Self) {
        let chip8_fontset: [u8; 80] =
[ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
];

        self.memory[..0x80].copy_from_slice(&chip8_fontset);
    }
}
