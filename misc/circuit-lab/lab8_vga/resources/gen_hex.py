#!/usr/bin/env python3
"""Resize a JPG to 640x480 (aspect-preserving scale + zero padding)
and write a $readmemh-compatible hex file, one RGB888 word per line."""

import argparse
from pathlib import Path

from PIL import Image

W, H = 640, 480
STRIDE = 1 << 9


def letterbox(path: Path, w: int = W, h: int = H) -> Image.Image:
    """Scale keeping aspect ratio, then pad with zeros (black)."""
    img = Image.open(path).convert("RGB")
    k = min(w / img.width, h / img.height)  # scale factor
    nw, nh = max(1, round(img.width * k)), max(1, round(img.height * k))
    small = img.resize((nw, nh), Image.Resampling.LANCZOS)
    canvas = Image.new("RGB", (w, h))  # zero-filled
    canvas.paste(small, ((w - nw) // 2, (h - nh) // 2))  # centered
    return canvas


if __name__ == "__main__":
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("src", type=Path, nargs="?", default=Path("input.jpg"))
    ap.add_argument("dst", type=Path, nargs="?", default=Path("image.hex"))
    args = ap.parse_args()

    data = letterbox(args.src).tobytes()  # "rrggbb" repeated
    blank = "000000"
    # address = h*STRIDE + v  ==>  outer loop over h, inner loop over v
    lines = [
        data[(v * W + h) * 3 : (v * W + h) * 3 + 3].hex() if v < H else blank
        for h in range(W)
        for v in range(STRIDE)
    ]
    args.dst.write_text("\n".join(lines) + "\n")
