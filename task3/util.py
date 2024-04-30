import numpy as np
import PIL.Image
from pathlib import Path

PIXEL_SIZE = 3
COLOR_NUMBER = 3
BMP_HEADER = 54
FLOAT32_SIZE = 4
CSVD_HEADER = 16


class SVD:
    def __init__(self, u: np.matrix, s: np.array, vh: np.matrix):
        self.U = u
        self.S = s
        self.VH = vh

    def matrix(self):
        return self.U @ np.diag(self.S) @ self.VH

    def compress(self, k):
        self.U = self.U[:, :k]
        self.S = self.S[:k]
        self.VH = self.VH[:k, :]
        return self


class SVDChannels:
    R: SVD
    G: SVD
    B: SVD

    def __init__(self, r, g, b):
        self.R = r
        self.G = g
        self.B = b

    def red(self):
        return self.R.matrix()

    def green(self):
        return self.G.matrix()

    def blue(self):
        return self.B.matrix()


class RGBChannels:
    R: np.matrix
    G: np.matrix
    B: np.matrix

    def set_colors(self, r: np.matrix, g: np.matrix, b: np.matrix):
        self.R = r
        self.G = g
        self.B = b

    def set_from_image(self, image: PIL.Image.Image):
        n, m = image.size
        self.R = np.matrix(np.zeros((n, m), dtype=int))
        self.G = np.matrix(np.zeros((n, m), dtype=int))
        self.B = np.matrix(np.zeros((n, m), dtype=int))
        for i in range(n):
            for j in range(m):
                self.R[i, j], self.G[i, j], self.B[i, j] = image.getpixel((i, j))

    def get_image(self):
        m, n = self.R.shape
        image = PIL.Image.new("RGB", (m, n))
        for i in range(m):
            for j in range(n):
                r, g, b = self.R[i, j], self.G[i, j], self.B[i, j]
                image.putpixel((i, j), (int(abs(r)), int(abs(g)), int(abs(b))))
        return image


class Serializer:
    FORMAT = "CSVD"

    def serialize(self, svd_channels: SVDChannels, path: Path):
        data = bytearray()
        m, k, n = svd_channels.R.U.shape[0], svd_channels.R.S.size, svd_channels.R.VH.shape[1]

        data.extend(self.FORMAT.encode("ASCII"))
        data.extend(np.uint32(m).tobytes())
        data.extend(np.uint32(k).tobytes())
        data.extend(np.uint32(n).tobytes())

        for channel in svd_channels.R, svd_channels.G, svd_channels.B:
            data.extend(channel.U.astype(np.float32).tobytes())
            data.extend(channel.S.astype(np.float32).tobytes())
            data.extend(channel.VH.astype(np.float32).tobytes())
        f = open(path, "wb")
        f.write(data)
        f.close()

    def deserialize(self, path: Path):
        f = open(path, "rb")
        buffer = f.read()
        f.close()
        if buffer[:4].decode("ASCII") != self.FORMAT:
            print("Wrong file format")
            exit(1)
        m = int(np.frombuffer(buffer, offset=4, dtype=np.uint32, count=1)[0])
        k = int(np.frombuffer(buffer, offset=8, dtype=np.uint32, count=1)[0])
        n = int(np.frombuffer(buffer, offset=12, dtype=np.uint32, count=1)[0])
        offset = 16
        svds = dict()
        for color in "RGB":
            u = np.matrix(np.frombuffer(buffer, offset=offset, dtype=np.float32, count=m * k).reshape(m, k))
            offset += m * k * FLOAT32_SIZE
            s = np.frombuffer(buffer, offset=offset, dtype=np.float32, count=k)
            offset += k * FLOAT32_SIZE
            vh = np.matrix(np.frombuffer(buffer, offset=offset, dtype=np.float32, count=k * n).reshape(k, n))
            offset += k * n * FLOAT32_SIZE
            svds[color] = SVD(u, s, vh)
        return SVDChannels(svds['R'], svds['G'], svds['B'])


class Compressor:
    @staticmethod
    def compress_bmp(image: PIL.Image.Image, N: float, svd_alg):
        m, n = image.size
        if N == 1:
            k = 0
        else:
            src_size = PIXEL_SIZE * m * n + BMP_HEADER
            k = min(m, n, int(src_size / (N * (m + n + 1) * COLOR_NUMBER * FLOAT32_SIZE + CSVD_HEADER)))
        colors = RGBChannels()
        colors.set_from_image(image)
        r = svd_alg(colors.R, k)
        g = svd_alg(colors.G, k)
        b = svd_alg(colors.B, k)
        return SVDChannels(r, g, b)

    @staticmethod
    def decompress_bmp(svd_channels: SVDChannels):
        rgb_channels = RGBChannels()
        rgb_channels.set_colors(svd_channels.red(), svd_channels.green(), svd_channels.blue())
        return rgb_channels.get_image()


def compress(in_file: Path, out_file: Path, N: float, svd_alg):
    image = PIL.Image.open(in_file)
    if image.format != "BMP" or len(image.getpixel((0, 0))) != COLOR_NUMBER:
        print("Wrong input file. Please give a path to BMP 24 bit file.")
        exit(1)
    Serializer().serialize(Compressor.compress_bmp(image, N, svd_alg), out_file)


def decompress(in_file: Path, out_file: Path):
    image = Compressor.decompress_bmp(Serializer().deserialize(in_file))
    image.save(out_file, "BMP")
