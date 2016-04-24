function err_norm=morlet_test(width, min_freq, max_freq, n_freqs, samplerate, winsize)

%width = 5;
%min_freq = 3.0;
%max_freq = 180.0;
%n_freqs = 8;
%samplerate = 1600.0;
%winsize = 4096;

wavelet_transformer = morlet_interface();
wavelet_transformer.init(width, min_freq, max_freq, n_freqs, samplerate, winsize);

signal = rand(1,winsize);

powers = zeros(1,n_freqs*winsize);
phases = zeros(1,n_freqs*winsize);
wavelet_transformer.multiphasevec(signal,powers,phases);

fs = logspace(log10(min_freq), log10(max_freq), n_freqs);
[tmp1,tmp,~] = multiphasevec2(fs, signal, samplerate, width);

powers2 = zeros(1,n_freqs*winsize);
phases2 = zeros(1,n_freqs*winsize);
for i=1:n_freqs
    for j=1:winsize
        powers2((i-1)*winsize+j) = tmp(i,j);
        phases2((i-1)*winsize+j) = tmp1(i,j);
    end
end

err_norm = norm(powers-powers2);
fprintf('Power error norm = %e\n', err_norm);

err_norm = norm(phases-phases2);
fprintf('Phase error norm = %e\n', err_norm);

end

