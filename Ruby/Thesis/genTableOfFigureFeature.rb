#!/usr/bin/ruby
require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/../lib/Utils.rb"
require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
STDOUT.sync = true

#cam_pairIdxLst=[4,12,15,22,24,34]
#cam_pair_cluster_condIdxLst=[[0,0],[12,1],[16,1],[19,0],[26,0],[30,0]]
cam_pair_cluster_condIdxLst=[[7,0],[31,0],[23,1],[28,1]]

featIdxLst=[0,1,4]

featLst=["BoVW SIFT","RGB Histogram","SDALF wHSV"]
setID="random0"
#cluster_cond="allInOne1"

res_str=""
capAlpLst=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s']
#カメラごとのループ


#--図開始
str = <<'EOS'
\begin{figure}[htpb]%--resultFeature
\begin{center}
EOS

res_str+=str
#puts res_str
#--行開始
#--列開始
cidx=0
2.times{|row|
	2.times{|col|
		#cam_pair=CamPairLst[cam_pairIdxLst[cidx]]
		cam_pairIdx,cluster_condIdx=cam_pair_cluster_condIdxLst[cidx]
		cam_pair=CamPairLst[cam_pairIdx]
		cluster_cond=Cluster_condLst[cluster_condIdx+1]
		cam_pair_str=cam_pair.gsub('cam','camera ')
		[['105',' A '],['106',' B '],['107',' C '],['108',' D '],['109',' E '],['110',' F '],['111',' A ']].each{|cam,cam_str|
			cam_pair_str=cam_pair_str.gsub(cam,cam_str)
		}
str = <<"EOS"
	\\begin{minipage}[b]{0.49\\textwidth}
	\\begin{center}
	\\includegraphics[width=\\widthOfResultsFeature]{results_20120220/Feature/param-Feature#{cluster_condIdx}_#{cam_pair}_#{cluster_cond}_#{setID}.eps}\\\\
	(#{capAlpLst[cidx]}) #{cam_pair_str}
	\\end{center}
	\\end{minipage}
EOS
res_str+=str
cidx+=1
	}#--列終わり
	res_str+="\\\\\n"
}#--行終わり
str = <<"EOS"
\\end{center}
\\vskip-\\lastskip
\\caption{--str--}
\\label{fig:resultsFeature}
\\end{figure}
EOS
res_str+=str
#図終わり

puts res_str