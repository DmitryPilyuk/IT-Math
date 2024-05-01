import argparse
from pathlib import Path
from svd_alghorithms import *
from util import compress, decompress


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--mode', required=True,
                        type=str,
                        choices=['compress', 'decompress'],
                        help='SVD compressor processing mode')
    parser.add_argument('--method', required=False, type=str,
                        choices=['simple', 'numpy', 'advanced'], help="Select compression algorithm.")
    parser.add_argument('--in_file', required=True, type=str, help="Path to input file")
    parser.add_argument('--out_file', required=False, type=str, help="Path to output file")
    parser.add_argument('--compression', required=False, type=float, help="Compression ratio")
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    if args.mode == "compress":
        if args.compression < 1:
            print("Compression ratio should be > 1")
            exit(1)
        if args.out_file is None:
            out_file = Path(args.in_file[:-3] + 'csvd')
        else:
            out_file = Path(args.out_file)
        if args.method == 'simple':
            svd_alg = power_svd
        elif args.method == 'numpy':
            svd_alg = numpy_svd
        elif args.method == 'advanced':
            svd_alg = advanced_svd
        else:
            print("Unknown method")
            exit(1)
        compress(Path(args.in_file), out_file, args.compression, svd_alg)
    elif args.mode == 'decompress':
        if args.out_file is None:
            out_file = Path(args.in_file[:-4] + 'bmp')
        else:
            out_file = args.out_file
        decompress(Path(args.in_file), out_file)
