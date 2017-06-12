

module Export


  def self.obj_to_array(obj_filename)
    
    # Arrays
    curr_name = ""
    positions = []
    normals = []
    uv = []
    index = []

    # Load obj file
    file = File.open(obj_filename, "r").read

    file.each_line do |l|

      line = l.strip

      if line.start_with?('o')
        curr_name = line.sub(/^o/, '')
        puts curr_name.strip
      end


      if line.start_with?('vt')

        tex_coord = line.sub(/^vt/, '')
        uv << tex_coord

      end


      if line.start_with?('vn')

        norm = line.sub(/^vn/, '')
        normals << norm

      end


      if line.start_with?('v')
        vertex = line.sub(/^v/, '')

        positions << vertex
        
      end

      if line.start_with?('f')
        face = line.sub(/^f/, '')

        index << face
      end

    end

    index = index.join(" ").split(" ")

    vert_info = "";
    vert_info << "constexpr float model[] = {"
    vert_info << "\n"

    index.each do |i|

      i = i.split("/")

      tab = "  "

      vert_info << tab
      vert_info << "/*vertex*/ " + positions[i[0].to_i - 1].split(" ").map{|k| "#{k}f,"}.join(" ").to_s
      vert_info << " /*uv*/ " + uv[i[1].to_i - 1].split(" ").map{|k| "#{k}f,"}.join(" ").to_s
      vert_info << " /*normal*/ " + normals[i[2].to_i - 1].split(" ").map{|k| "#{k}f,"}.join(" ").to_s
      vert_info << " \n"

    end 

    vert_info << "};\n"
  
    vert_info << "\n"
    vert_info << "constexpr uint32_t vertex_count = "
    vert_info << index.length.to_s
    vert_info << ";\n"

    File.write('out.hpp', vert_info)

  end


end

Export.obj_to_array(ARGV[0]);
