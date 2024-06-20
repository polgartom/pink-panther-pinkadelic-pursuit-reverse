const ll = console.log;

const HEADER_SIZE = 12;
let BYTES_PER_PIXEL = 2;
let CHUNK_END_OFFSET = 0;

let file = null; // from url query param ?tex=
let pixels = [];
let palette = {};
let data = '';
let bytes = [];

Array.prototype.get8Int = function() {
    let b = this.splice(0, 1);
    return (b[0] & 0xFF);
}

Array.prototype.getL16Int = function() { // Little endian
    let b = this.splice(0, 2);
    return ( (b[1] << 8) | b[0] ) & 0xFF_FF;
}

Array.prototype.getL24Int = function() { // Little endian
    let b = this.splice(0, 3);
    return ( (b[2] << 16) | (b[1] << 8) | b[0] ) & 0xFF_FF_FF;
}

Array.prototype.getL32Int = function() { // Little endian
    let b = this.splice(0, 4);
    return ( (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0] ) & 0xFF_FF_FF_FF;
}

function decode_16bit_rgb(rgb565) {
    // @Todo: Not 100% right
    blue = (rgb565 >> 10) & 0b11111;
    green = (rgb565 >> 4) & 0b111111;
    red = rgb565 & 0b11111;

    blue = Math.round(blue / 31 * 255);
    green = Math.round(green / 63 * 255);
    red = Math.round(red / 31 * 255);

    return [red, green, blue];
}


function preload() {
    file = new URLSearchParams(window.location.search).get('tex');
    if (!file) {
        window.alert('PLEASE SELECT A TEX FILE!');
        return;
    }

    tex = loadBytes(file);
}

function unpack_16(n) {
    return [
        (n >> 8 ) & 0xFF,
        (n >> 0 ) & 0xFF,
    ];
}

function unpack_24(n) {
    return [
        (n >> 16 ) & 0xFF,
        (n >> 8  ) & 0xFF,
        (n >> 0  ) & 0xFF,
    ];
}

function unpack_32(n) {
    return [
        (n >> 24) & 0xFF,
        (n >> 16) & 0xFF,
        (n >> 8 ) & 0xFF,
        (n >> 0 ) & 0xFF,
    ];
}

function setup() {
    bytes = [...tex.bytes];

    header = bytes.splice(0, HEADER_SIZE);

    BYTES_PER_PIXEL = header.getL16Int();
    W = header.getL16Int();
    H = header.getL16Int();
    ll(`${W} x ${H} / ${BYTES_PER_PIXEL}`);
    
    // if ((bytes.length % BYTES_PER_PIXEL) !== 0) {
    //     alert("(bytes.length % BYTES_PER_PIXEL) !== 0");
    // }

    ll(`total bytes: ${bytes.length} ; W*H: ${bytes.length/BYTES_PER_PIXEL} / ${W*H}`);

    header.getL16Int(); // ???
    CHUNK_END_OFFSET = header.getL32Int();

    const PALETTE_MODE = BYTES_PER_PIXEL === 1;

    if (PALETTE_MODE) {
        // In palette mode, we need to construct the palette by calculating its offset.
        // The palette address will be after the main chunk, where each byte represents the relative offset on the palette.
        let chunk = bytes.splice(0, CHUNK_END_OFFSET-HEADER_SIZE);
        while (chunk.length) {
            let color_index = chunk.get8Int();
            if (palette[color_index] === undefined) {
                // Fetch and create new color from palette
                // palette color offset (absolute address) calc as: chunk_size_in_bytes + (color_index * bytes_per_pixel)
                let b = bytes.slice(color_index*2, color_index*2+2);
                b = ( (b[1] << 8) | b[0] ) & 0xFFFF; // Big to little endian
                let _c = decode_16bit_rgb(b);
                palette[color_index] = color(..._c);
            }
            pixels.push(palette[color_index]);
        }

    } else {
        // Every 2 bytes represent a pixel color
        while (bytes.length) {
            let _b = bytes.splice(0, 2);
            b = ( (_b[1] << 8) | _b[0] ) & 0xFFFF; // Big to little endian
            _c = decode_16bit_rgb(b);
            let c = color(..._c);
            pixels.push(c);
        }
    }

    console.log(W, H);

    createCanvas(W, H);
    colorMode(RGB);
}
  
function draw() {
    background(50, 50, 50);
    
    let i = 0;
    for (let y = 0; y < H; y += 1) {
        for (let x = 0; x < W; x += 1) {
            let c = pixels[i];
            set(x, y, c);
            i += 1;
        }
    }

    updatePixels();
}