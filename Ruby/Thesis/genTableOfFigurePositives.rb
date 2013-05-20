#!/usr/bin/ruby
require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/../lib/Utils.rb"
require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
STDOUT.sync = true

cam_pairIdxLst=[4,12,15,22,24,34]
featIdxLst=[0,1,4]
featLst=["BoVW SIFT","RGB Histogram","SDALF wHSV"]
setID="random0"
res_str=""
capAlpLst=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s']
cluster_cond=Cluster_condLst[1]
featureMetricsStr="#{FeatureMetrics_ParamLst[0][0]}_#{FeatureMetrics_ParamLst[0][1]}"
def getCamPairStr(cam_pair)
	cam_pair_str=cam_pair.gsub('cam','camera')
	[['105',' A '],['106',' B '],['107',' C '],['108',' D '],['109',' E '],['110',' F ']].each{|cam,cam_str|
		cam_pair_str=cam_pair_str.gsub(cam,cam_str)
	}
	return cam_pair_str
end

#--------------------------------------------------------------------------------
	#--図開始
str = <<'EOS'
\begin{figure}[htpb]
\begin{center}
EOS
res_str+=str
#カメラごとのループ
#cam_pairIdxLst.each{|cidx|


	#puts res_str
	#--行開始
	#--列開始
	#featIdxLst.each_with_index{|fidx,col|
idx=0
3.times{|row|
	2.times{|i|
		cidx=cam_pairIdxLst[idx]
		cam_pair=CamPairLst[cidx]
		cam_pair_str=getCamPairStr(cam_pair)
str = <<"EOS"
	\\begin{minipage}[b]{0.49\\textwidth}
	\\begin{center}
	\\includegraphics[width=\\widthOfResultsDouble]{results/Positive/param-Positive_#{setID}_#{cluster_cond}_#{featureMetricsStr}_#{cam_pair}.eps}\\\\
	(#{capAlpLst[idx]}) #{cam_pair_str} cluster#{i+1}
	\\end{center}
	\\end{minipage}
EOS
		res_str+=str
		idx+=1
	}#--列終わり
	res_str+="\\\\\n" if row!=2
}#--行終わり
str = <<"EOS"
\\end{center}
\\vskip-\\lastskip
\\caption{--str--}
\\label{fig:resultsPositive}
\\end{figure}
EOS
res_str+=str
#図終わり
puts res_str
