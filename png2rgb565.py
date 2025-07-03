from PIL import Image


def rgb888_to_rgb565(r: int, g: int, b: int) -> int:
    """Wandelt 8-Bit-RGB in 16-Bit-RGB565 um."""
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def swap_bytes(value: int) -> int:
    """Vertauscht die beiden Bytes eines 16-Bit-Werts."""
    return ((value & 0xFF) << 8) | ((value >> 8) & 0xFF)

def image_to_rgb565_array(image_path: str, array_name: str = "image", do_swap_bytes: bool = True) -> None:
    """Liest ein PNG ein und gibt ein C-Array im RGB565-Format mit vertauschten Bytes aus."""
    img = Image.open(image_path).convert("RGBA")
    pixels = list(img.getdata())
    width, height = img.size

    print(f"// size: {width}x{height}")
    print(f"const uint16_t {array_name}[] PROGMEM = {{")
    for i, (r, g, b, a) in enumerate(pixels):
        rgb565 = rgb888_to_rgb565(r, g, b)
        if a < 128:
            r, g, b = 0, 0, 0
        if do_swap_bytes:
            swapped = swap_bytes(rgb565)
        else:
            swapped = rgb565
        if i % 12 == 0:
            print("    ", end="")
        print(f"0x{swapped:04X}, ", end="")
        if (i + 1) % 12 == 0:
            print()
    print("\n};")


if __name__ == "__main__":
    # Beispielaufruf:
    # image_to_rgb565_array("logo.png", "img_logo")
    import sys
    if len(sys.argv) < 2:
        print("Nutzung: python png2rgb565.py <bild.png> [arrayname]")
    else:
        image_to_rgb565_array(sys.argv[1], sys.argv[2] if len(sys.argv) > 2 else "image")
