#!/usr/bin/ruby
require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/../lib/Utils.rb"
require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
STDOUT.sync = true

cam_pairIdxLst=[0,12,16,19,26,30]
featIdxLst=[0,1,4]
featLst=["BoVW SIFT","RGB Histogram","SDALF wHSV"]

res_str=""
capAlpLst=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s']
#カメラごとのループ
cam_pairIdxLst.each{|cidx|
cam_pair=CamPairLst[cidx]
cam_pair_str=cam_pair.gsub('cam','camera')
[['105',' A '],['106',' B '],['107',' C '],['108',' D '],['109',' E '],['110',' F ']].each{|cam,cam_str|
	cam_pair_str=cam_pair_str.gsub(cam,cam_str)
}
#--図開始
#--行開始
str = <<'EOS'
\begin{figure}[htpb] %--resultsClustering
\begin{center}
EOS
res_str+=str
#puts res_str
#--列開始
featIdxLst.each_with_index{|fidx,col|
str = <<"EOS"
	\\begin{minipage}[b]{0.32\\textwidth}
	\\begin{center}
	\\includegraphics[width=\\widthOfResultsClustering]{results/Clustering/param-Clustering_#{FeatureMetrics_ParamLst[fidx][0]}_#{FeatureMetrics_ParamLst[fidx][1]}_#{cam_pair}.eps}\\\\
	(#{capAlpLst[col]}) #{featLst[col]}
	\\end{center}
	\\end{minipage}
EOS
res_str+=str
}#--列終わり
str = <<"EOS"
\\end{center}
\\vskip-\\lastskip
\\caption{--str--#{cam_pair_str}}
\\label{fig:resultsClustering_#{cidx}}
\\end{figure}
EOS
res_str+=str
#--行終わり
#図終わり
}
puts res_str