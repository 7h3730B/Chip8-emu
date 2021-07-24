mod constants;
mod chip8;

pub struct Cpu {
    pub pc: u16,
    pub I: u16;
    pub sp: u8,
    pub registers: [u8; constants::N_REGISTERS],
}