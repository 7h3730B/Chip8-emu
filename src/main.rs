mod chip8;

fn main() {
    let chip = chip8::Chip8::new();
    
    chip.setup_from_file( "roms/BLINKY");
}
