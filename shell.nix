with import <nixpkgs> {};
let
in
stdenv.mkDerivation {
  name = "PNL";
  hardeningDisable = [ "all" ];
  buildInputs = [ valgrind readline ];
}
