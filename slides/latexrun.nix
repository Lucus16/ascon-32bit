{ stdenvNoCC, fetchFromGitHub, python3, tetex }:

stdenvNoCC.mkDerivation {
  name = "latexrun";
  src = fetchFromGitHub {
    owner = "aclements";
    repo = "latexrun";
    rev = "38ff6ec2815654513c91f64bdf2a5760c85da26e";
    sha256 = "0xdl94kn0dbp6r7jk82cwxybglm9wp5qwrjqjxmvadrqix11a48w";
  };

  buildInputs = [ python3 ];

  buildPhase = "";
  installPhase = ''
    mkdir -p $out/bin
    cp latexrun $out/bin/latexrun
    chmod +x $out/bin/latexrun
  '';
}
