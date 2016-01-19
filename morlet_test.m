wavelet_transformer = morlet_interface();
wavelet_transformer.init();

signal = rand(1,4096);
%for i=1:4096
%    signal(i) = sin((i-1)*pi/317.0);
%end

powers = zeros(1,8*4096);
wavelet_transformer.multiphasevec(signal,powers);

fs = logspace(log10(3), log10(180), 8);
[~,tmp,~] = multiphasevec2(fs, signal, 1000.0, 5);

powers2 = zeros(1,8*4096);
for i=1:8
    for j=1:4096
        powers2((i-1)*4096+j) = tmp(i,j);
    end
end

disp(powers-powers2);
