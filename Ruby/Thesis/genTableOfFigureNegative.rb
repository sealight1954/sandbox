#!/usr/bin/ruby
require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/../lib/Utils.rb"
require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
STDOUT.sync = true

cam_pairIdxLst=[0,12,16,19,26,30]
cam_pair_cluster_condIdxLst=[[0,0],[12,1],[16,1],[19,0],[26,0],[30,0]]
featIdxLst=[0,1,4]
featLst=["BoVW SIFT","RGB Histogram","SDALF wHSV"]
setID="random0"
res_str=""
capAlpLst=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s']
#カメラごとのループ
cam_pair_cluster_condIdxLst.size.times{|cidx|
#cam_pair_cluster_condIdxLst.each{|cam_pairIdx,cluster_condIdx|
		cam_pairIdx,cluster_condIdx=cam_pair_cluster_condIdxLst[cidx]
		cam_pair=CamPairLst[cam_pairIdx]
		cluster_cond=Cluster_condLst[cluster_condIdx+1]
#cam_pairIdxLst.each{|cidx|
#cam_pair=CamPairLst[cidx]
cam_pair_str=cam_pair.gsub('cam','camera')
[['105',' A '],['106',' B '],['107',' C '],['108',' D '],['109',' E '],['110',' F ']].each{|cam,cam_str|cam_pair_str=cam_pair_str.gsub(cam,cam_str)}
#--図開始
#--行開始
str = <<'EOS'
\begin{figure}[htpb] %--resultsNegative
\begin{center}
EOS
res_str+=str
#puts res_str
#--列開始
featIdxLst.each_with_index{|fidx,col|
#param-Negative1_cam107-105_BoVWSIFT_histIntsec_p1_cam105-110.11.05_RGBHistkmeans2Median1_random0.jpg
str = <<"EOS"
	\\begin{minipage}[b]{0.32\\textwidth}
	\\begin{center}
	\\includegraphics[width=\\widthOfResultsNegative]{results/Negative/param-Negative#{cluster_condIdx}_#{cam_pair}_#{FeatureMetrics_ParamLst[fidx][0]}_#{FeatureMetrics_ParamLst[fidx][1]}_#{cluster_cond}_#{setID}.eps}\\\\
	(#{capAlpLst[col]}) #{featLst[col]}
	\\end{center}
	\\end{minipage}
EOS
res_str+=str
}#--列終わり
str = <<"EOS"
\\end{center}
\\vskip-\\lastskip
\\caption{\caption#{param} #{cam_pair_str}}
\\label{fig:resultsNegative_#{cidx}}
\\end{figure}
EOS
res_str+=str
#--行終わり
#図終わり
}
puts res_str