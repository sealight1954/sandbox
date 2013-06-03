#!/usr/bin/ruby
#�t�H���_�����邩�ǂ����`�F�b�N���ĂȂ���΍쐬
class String
	def chkMkdir
		if !(File.exist?(self)) then
			Dir.mkdir(self)
		end
		return self
	end
	

	def getFileLst(ext = ".jpg")
		str=`ls #{self}/ | grep #{ext}`.split("\n")
		return str
	end
	def getFilePathLst(ext = ".jpg")
		directory=(self+"/").gsub("//","/")
		str=`ls #{directory} | grep #{ext}`.split("\n")
		res_array=[]
		str.each{|filename|
		res_array.push(directory+filename)
		}
		return res_array
	end
	def getFolderLst
		dstLst=[]
		temp=`ls -F #{self} | grep /`
		temp.each{|folder|
			dstLst.push(folder.chomp[0..folder.size-3])#chomp�̈ʒu�ɒ���
		}
		return dstLst
	end

	
	# self�t�H���_���̃T�u�t�H���_���X�g folderLst�̂��ꂼ��ɂ��ăt�@�C�����݂̂̃��X�g�쐬
	def getFilenameLstLst(folderLst,ext = ".jpg")
		dstLstLst=[]
		folderLst.each{|folder|
			str=`ls #{self}#{folder}/ | grep #{ext}`
			imgLst=[]
			str.each{|img|
				imgLst.push(img.chomp)
			}
			dstLstLst.push(imgLst)
		}
		return dstLstLst
	end
	# self�t�H���_���̃T�u�t�H���_���X�g�̂��ꂼ��ɂ��ā@�t���p�X�̃��X�g�쐬
	def getFullPathLstLst(folderLst,ext = ".jpg")
		dstLstLst=[]
		folderLst.each{|folder|
			str=`ls #{self}#{folder}/ | grep #{ext}`
			imgLst=[]
			str.each{|img|
				imgLst.push("#{self}#{folder}/"+img.chomp)
			}
			dstLstLst.push(imgLst)
		}
		return dstLstLst
	end
	
	def getArray
		array=[]
		f = open(self)
		# str=File.open(self).read

		f.each{|row|
			array.push(row.chomp)
		}
		return array
	end
	def get2dFloatArray(sep=" ")
		raise ArgumentError, "dosen't exist: #{self}" if !File.exists?(self)
		array=[]
		str=File.open(self).read
		str.each{|row|
			row_array=[]
			row.chomp.split(sep).each{|cell|
				row_array.push(cell.to_f)
			}
			array.push(row_array)
		}
		return array
	end
	#1:key value�̃e�L�X�g�t�@�C����ǂݍ���
	def getFloatHash(sep=" ")
		res_hash={}
		str=File.open(self).read
		str.each{|row|
			##row_array=
			#idx= row_array[0]=="" ? 1 : 0
			key,val=row.split(sep)
			res_hash[key]=val.to_f
		}
		return res_hash
	end
		#"yyyy-mm-dd hh:MM:ss"��Time�I�u�W�F�N�g�ɕϊ�
	def toTime(sep1 = " " , sep2 = "-" , sep3 = ":")
		date,time=self.split(sep1)
		d=date.split(sep2)
		t=time.split(sep3)
		#p d
		#p t
		return Time.gm(d[0],d[1],d[2],t[0],t[1],t[2])
	end

end


class Array
	#order: ID�̏�����value�̍~��
	#ID���ɕ���ł���value�̔z���ID�AOrder�t���̔z��ŕԂ�
	def getIdOrderValues(order = "value")
		array=[]
		0.upto(self.size-1){|i|
			temp=[i,self[i]] # id,value�̗v�f
			array.push(temp)
		}
		res_array=[]
		array.sort{|a,b|a[1] <=> b[1]}.reverse.each_with_index{|line,idx| #id,value ::value �̍~��
			res_array.push([idx,line[0],line[1]])#order,id(dim),value
		}
		return res_array.sort{|a,b| a[1] <=> b[1] } if order=="id" #id(dim)�̏���
		return res_array
	end
	
	#�S�v�f�̘a��Ԃ�
	def sum
		
		if self[0].instance_of?(Float)
			sum_num=0.0
			self.each{|num|sum_num+=num}
		elsif self[0].instance_of?(Integer)
			sum_num=0
			self.each{|num|sum_num+=num}
		else
			return nil
		end
		return sum_num
	end
	
	def And(array)
		raise ArgumentError, "dosen't exist: #{filepath}" if !(array.instance_of?(Array))
		res_array=[]
		self.each{|cell1|
			array.each{|cell2|
			res_array.push(cell1) if cell1==cell2
			}
		}
		return res_array
	end
	
	#�P�v�f�P�s�Ɋi�[
	def save(filepath,sep=" ")
		File.open(filepath,"w"){|fout|
			self.each{|line|
				if false
				elsif line.instance_of?(Array)
					str=line.join(sep)
					fout.write(str)
					fout.write("\n")
				elsif !(line.instance_of?(String)) && !(line.instance_of?(Integer)) && !(line.instance_of?(Float)) && !(line.instance_of?(Fixnum))
					p line.type
					p line
					raise ArgumentError, "line is not string or integer or float" 
				else
				fout.write("#{line}\n")
				end
			}
		}
	end
	
	def save2dArray(filepath,sep=" ")
		File.open(filepath,"w"){|fout|
			self.each{|row|
				str=row.join(sep)
				fout.write(str)
				fout.write("\n")
			}
		}
	end
	
	#�\�[�g�������̎w��v�f�̏��ʂ�Ԃ�#default=����
	def rank(idx,order="asc")
		rank_array=[]
		self.each_with_index{|val,i|
			rank_array.push([val,i])
		}
		if order!="dsc"
			return rank_array.sort{|a,b| a[0]<=> b[0]}.index{|item|item[1]==idx}
		else
			return self.size - 1 - rank_array.sort{|a,b| a[0]<=> b[0]}.index{|item|item[1]==idx}
		end
	end
	
end

class Time
	def toStr
		return self.strftime("%Y-%m-%d %H:%M:%S")
	end


end

class HashArray

	# 1�s�ڂ��t�B�[���h�l�Ƃ��āA2�s�ڈȍ~��l�Ƃ��ēǂݍ���
	def initialize(filename)
		File.open(filename){|f|
			line=f.gets
			@fielsds = line.chomp.split(",")
			while line = f.gets
				@dataArray.push(line.chomp.split(","))
			end
		}
	end
	def select(field,value)
		idx=@fields.index(field)
		raise ArgumentError, "not exist field name" if idx==nil
		temp=[]
		@dataArray.each{|row|
			temp.push(row) if row[idx]==value
		}
		@dataArray=temp
		return self
	end
	
	def getArray
		return @dataArray
	end
	def getIdx(field)
		return @fields.index(field)
	end
	
	@dataArray
	@fields
end

#--�t�H���_���̃t�@�C�����폜���ăt�H���_���폜
def remove_folder(_rmfolder)
		rmimg_lst=`ls #{_rmfolder.chomp}`
		rmimg_lst.each{|_img|
			#puts "rm #{_img.chomp}"
			`rm #{_rmfolder.chomp}#{_img.chomp}`
		}
		`rm -r #{_rmfolder.chomp}`
end


def waitKey(strArray=[""])
		messege=""
		strArray.each{|str|messege+="/"+str}
		puts "input #{messege}"
		
		while temp = STDIN.gets
				line=temp.chomp
		    p line
		    flg=false
		    strArray.each{|str|flg=true if str==line}
		    if flg
		        break;
		    end
		end
		return  line
end
