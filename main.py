import os, zipfile,sys,time,shutil
'''
  make_zip
  功能: 打包文件夹到指定位置
  参数: src_dir
          需要打包的文件夹路径
        dst_dir
          zip压缩包生成路径
        output_filename
          生成压缩包的名称
'''
def make_zip(src_dir,dst_dir,output_filename):
  zipf = zipfile.ZipFile(output_filename, 'w',zipfile.ZIP_DEFLATED)
  pre_len = len(os.path.dirname(src_dir))
  for parent, dirnames, filenames in os.walk(src_dir):
    for filename in filenames:
      pathfile = os.path.join(parent, filename)
      arcname = pathfile[pre_len:].strip(os.path.sep)
      zipf.write(pathfile, arcname)
  zipf.close()
  shutil.move(f'{output_filename}',dst_dir)

def rm_file(dst_dir):
  os.remove(dst_dir)

def unzip_file(zip_src, dst_dir):    
    fz = zipfile.ZipFile(zip_src, 'r')
    for file in fz.namelist():
        fz.extract(file, dst_dir)

def isexists_dir_Create(path):
     if not os.path.exists(path):
        os.makedirs(path)
'''
  search_zip
  功能: 返回指定目录下zip文件的路径(列表)
  参数: filepath
          指定目录

'''
def search_zip(filepath):
    zip_path = []
    pathdir = os.listdir(filepath)
    for s in pathdir:
        newdir = os.path.join(filepath, s)
        if os.path.isfile(newdir):
            if os.path.splitext(newdir)[1] == ".zip":
                zip_path.append(newdir)
        elif os.path.isdir(newdir):
            search_zip(newdir)
    return zip_path

src_path = os.path.expanduser('~\\AppData\\Roaming\\EldenRing')
dst_path = os.path.expanduser('~\\Documents\\EldenRingBackFile\\')
zip_name = f'{time.strftime("%Y-%m-%d %H时%M分%S秒", time.localtime())}.zip'

isexists_dir_Create(dst_path)

while True:
  try:
    cmd = int(input('输入数字以继续操作(按 0 退出程序):\n1. 快速存档\n2. 快速换档\n3. 删除存档\n'))
    if cmd not in range(4):
      print('命令无效, 请重新输入')
      continue
  except Exception:
    print('输入有误')
    continue
  break
if cmd==0:
  sys.exit(0)
elif cmd==1:
  input_name = input('输入存档名(直接回车则默认命名为时间):')
  if input_name!='':
    zip_name = input_name + '.zip'
  make_zip(src_path,dst_path,zip_name)
  print('快速存档完成,存档位于%s'%dst_path)
  msg = input('按回车键退出...')
  sys.exit(0)
elif cmd==2:
  print('以下为您之前的存档, 请选择一个回档(输入存档前面的数字,按 0 退出程序):')
  zips_path = search_zip(dst_path)
  for i in range(len(zips_path)):
    print(str(i+1)+'. '+zips_path[i].replace(dst_path,''))
  while True:
    try:
      cmd = int(input())
      print('您选择了存档%d'%cmd)
      if cmd not in range(len(zips_path)+1):
        print('无此存档, 请重新输入')
        continue
    except Exception:
      print('输入有误, 请重新输入')
      continue
    break
  if cmd==0:
    sys.exit(0)
  else:
    shutil.rmtree(src_path)
    unzip_file(f'{zips_path[cmd-1]}',src_path.replace('EldenRing',''))
    print('换挡完成')
  msg = input('按回车键退出...')
  sys.exit(0)
else:
  print('以下为您之前的存档, 请选择一个或多个删除(输入存档前面的数字, 删除多个时以空格隔开, 按 0 退出程序):')
  zips_path = search_zip(dst_path)
  for i in range(len(zips_path)):
    print(str(i+1)+'. '+zips_path[i].replace(dst_path,''))
  rm_index = []
  while True:
    try:
      cmd = input()
      cmd = cmd.split(' ')    
      for i in cmd:
        if i != ' ':
          rm_index.append(int(i))
    except Exception:
      print('命令无效, 重新输入')
      rm_index.clear()
      continue
    if min(rm_index) < 0 or max(rm_index) > len(zips_path):
      print('指令越界, 重新输入')
      rm_index.clear()
      continue
    break
  if min(rm_index)==0:
    sys.exit(0)
  for i in rm_index:
    os.remove(zips_path[i-1])
  zips_path = search_zip(dst_path)
  print('已删除存档%s更新后的存档为:'%str(rm_index))
  for i in range(len(zips_path)):
    print(str(i+1)+'. '+zips_path[i].replace(dst_path,''))
  msg = input('按回车键退出...')
  sys.exit(0)