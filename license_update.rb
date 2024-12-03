#! ruby

require 'find'

def updateFiles
version = "10"    
    Find.find('./') do |f|
        if (File.basename(f).include?("FreeRTOSConfig"))
            File.open(f,'r') do |file|
                while line = file.gets
                    if line.include?("FreeRTOS ")
                        index = line.index("V")
                        version = line[index+1..index+3]
                        break
                    end
                end
            end
            vnum = version.to_i
            if vnum < 10
                puts "Version not actual (#{vnum}), rewriting..."
                rewrite(f)
            end
            puts "#{f} version: #{version}"
        end
    end
end

def rewrite(file)
    license = ""
    text = ""
    lineNum = 0
    File.open('license.txt','r') do |f|
        license = f.read
    end
    File.open(file,'r') do |input|
        while line = input.gets
            if line.include?("*/")
                break
            end
        end
        text = input.read
    end
    File.open(file,'w+') do |output|
        output.write(license)
        output.write(text)
    end
end

updateFiles